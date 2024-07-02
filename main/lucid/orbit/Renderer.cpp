#include "Renderer.h"
#include "Ephemeris.h"
#include "StarCatalog.h"
#include "Frame.h"
#include "Utility.h"
#include "Constants.h"
#include <lucid/gigl/Model.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Camera2D.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/TargetReader2D.h>
#include <lucid/gal/RenderTarget2D.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/System.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Algorithm.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

inline LUCID_GAL::Pipeline &galPipeline()
{
	return LUCID_GAL::Pipeline::instance();
}

inline LUCID_ORBIT::StarCatalog &theStarCatalog()
{
	return LUCID_ORBIT::StarCatalog::instance();
}

inline LUCID_ORBIT::Ephemeris &theEphemeris()
{
	return LUCID_ORBIT::Ephemeris::instance();
}

template<typename T> struct NullSort
{
	NullSort(LUCID_GAL::Vector3 const & /* dummy */)
	{
	}

	bool operator()(T const &, T const &) const
	{
		return false;
	}
};

template<typename T> struct Back2Front
{
	LUCID_GAL::Vector3 viewPosition;

	Back2Front(LUCID_GAL::Vector3 const &viewPosition)
		: viewPosition(viewPosition)
	{
	}

	bool operator()(T const &lhs, T const &rhs) const
	{
		return LUCID_MATH::lsq(lhs.position - viewPosition) > LUCID_MATH::lsq(rhs.position - viewPosition);
	}
};

template<typename T> struct Front2Back
{
	LUCID_GAL::Vector3 viewPosition;

	Front2Back(LUCID_GAL::Vector3 const &viewPosition)
		: viewPosition(viewPosition)
	{
	}

	bool operator()(T const &lhs, T const &rhs) const
	{
		return LUCID_MATH::lsq(lhs.position - viewPosition) < LUCID_MATH::lsq(rhs.position - viewPosition);
	}
};

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

///
///
///

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	shutdown();
}

void Renderer::evaluate(DynamicPoint *point)
{
	LUCID_PROFILE_SCOPE("Renderer::evaluate(DynamicPoint)");

	///	TBD: implement
}

void Renderer::evaluate(OrbitalBody *body)
{
	LUCID_PROFILE_SCOPE("Renderer::evaluate(OrbitalBody)");

	///	TBD: data drive line width and color
	///	TBD: data drive domain of orbit

	PhysicalProperties const &physicalProperties = body->physicalProperties;
	RenderProperties const &renderProperties = body->renderProperties;
	Elements const *elements = body->elements;

	LUCID_GAL::Vector3 bodyPosition = adaptiveScale(LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition);

	MeshInstance bodyInstance;
	bodyInstance.id = (SELECT_FRAME << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
	bodyInstance.position = bodyPosition;
	bodyInstance.scale = adaptiveScale(physicalProperties.radius);
	bodyInstance.rotation = LUCID_GAL::Quaternion(0, 0, 0, 1);
	bodyInstance.color = renderProperties.color;
	bodyInstance.parameters = renderProperties.parameters;
	_sceneBatch.addInstance(renderProperties.model, bodyInstance);

	if (!renderProperties.showOrbit)
		return;

	Frame const *centerFrame = body->centerFrame;
	LUCID_GAL::Vector3 centerPosition = adaptiveScale(LUCID_MATH::lerp(_interpolant, centerFrame->absolutePosition[0], centerFrame->absolutePosition[1]));

	LUCID_GAL::Scalar  a = adaptiveScale(LUCID_MATH::lerp(_interpolant, elements[0].A, elements[1].A));
	LUCID_GAL::Scalar  e = cast(LUCID_MATH::lerp(_interpolant, elements[0].EC, elements[1].EC));
	LUCID_GAL::Scalar hu = a * (1.f - e * e);

	LUCID_GAL::Quaternion rotation = LUCID_MATH::slerp(
		cast(_interpolant).value,
		LUCID_MATH::quaternionFromMatrix(cast(rotationFromElements(elements[0]))),
		LUCID_MATH::quaternionFromMatrix(cast(rotationFromElements(elements[1])))
	);

	MeshInstance orbitInstance;
	orbitInstance.id = (SELECT_ORBIT << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
	orbitInstance.position = centerPosition;
	orbitInstance.scale = 3.f;
	orbitInstance.rotation = rotation;
	orbitInstance.color = renderProperties.orbitHighlight ? LUCID_GAL::Color(1.f, 1.f, 1.f, 1.f) : LUCID_GAL::Color(0, 0, 1, 1);
	orbitInstance.parameters = LUCID_GAL::Vector4(hu, e, 0.f, constants::two_pi<float32_t>);
	_orbitBatch.addInstance(_orbitMesh, orbitInstance);
}
	  
void Renderer::evaluate(DynamicBody *body)
{
	LUCID_PROFILE_SCOPE("Renderer::evaluate(DynamicBody)");
	///	TBD: implement
}

void Renderer::evaluate(CameraFrame *camera)
{
	LUCID_PROFILE_SCOPE("Renderer::evaluate(CameraFrame)");
	///	TBD: implement
}

void Renderer::initialize(std::string const &path)
{
	shutdown();

	_renderContext = LUCID_GIGL::Context(path);

	LUCID_GAL::System &galSystem = LUCID_GAL::System::instance();
	_width = galSystem.width();
	_height = galSystem.height();

	_starCount = theStarCatalog().count();
	_starMesh.reset(LUCID_GIGL::Mesh::create("content/star.mesh"));
	_starInstances.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, int32_t(_starCount), sizeof(StarInstance)));

	StarInstance *starInstances = (StarInstance *)(_starInstances->lock());
	for (size_t i = 0; i < _starCount; ++i)
	{
		StarCatalog::Entry const &entry = theStarCatalog()[i];
		StarInstance &instance = starInstances[i];

		instance.id = uint32_t((SELECT_STAR << SELECT_SHIFT) | i);

		/// TBD: need a lookup mapping star type to texcoord into spectral texture...
		instance.parameters.x = 0.01f * (rand() % 100);
		instance.parameters.y = float32_t(entry.right_ascension);
		instance.parameters.z = float32_t(entry.declination);
		instance.parameters.w = entry.magnitude;
	}
	_starInstances->unlock();

	_sceneBatch.initialize();
	_orbitBatch.initialize();

	/// test {
	///	need a data driven method for registering these... 
	/// ...just read the ephemeris stupid!!!
	_sceneBatch.createBatch<MeshInstance, Back2Front<MeshInstance> >(LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>("content/atmosphere.mesh"), BATCH_MAXIMUM);
	_sceneBatch.createBatch<MeshInstance, Front2Back<MeshInstance> >(LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>("content/hemisphere.mesh"), BATCH_MAXIMUM);
	/// } test
		 
	_orbitMesh = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>("content/orbit.mesh");
	_orbitBatch.createBatch<MeshInstance, Back2Front<MeshInstance> >(_orbitMesh, BATCH_MAXIMUM);

	_selectTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
	_selectReader.reset(LUCID_GAL::TargetReader2D::create(_selectTarget.get(), _width, _height));

	_colorTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_glowTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[0].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[1].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));

	_clear.reset(LUCID_GIGL::Mesh::create("content/clear.mesh"));
	_copy .reset(LUCID_GIGL::Mesh::create("content/copy.mesh"));
	_blur .reset(LUCID_GIGL::Mesh::create("content/blur.mesh"));
	_post .reset(LUCID_GIGL::Mesh::create("content/post.mesh"));
	_fxaa .reset(LUCID_GIGL::Mesh::create("content/fxaa.mesh"));

	_starParameters.sphereRadius = _starMesh->material()->program()->lookup("sphereRadius");
	_starParameters. spriteScale = _starMesh->material()->program()->lookup( "spriteScale");

	_copyParameters.  theSource = _copy->material()->program()->lookup(  "theSource");

	_blurParameters.texelOffset = _blur->material()->program()->lookup("texelOffset");
	_blurParameters.  theSource = _blur->material()->program()->lookup(  "theSource");

	_postParameters.colorTarget = _post->material()->program()->lookup("colorTarget");
	_postParameters. glowTarget = _post->material()->program()->lookup( "glowTarget");

	_fxaaParameters.colorTarget = _fxaa->material()->program()->lookup("colorTarget");
	_fxaaParameters. glowTarget = _fxaa->material()->program()->lookup( "glowTarget");
}

void Renderer::shutdown()
{
	_starCount = 0;
	_starInstances.reset();
	_starMesh.reset();

	_orbitBatch.shutdown();
	_sceneBatch.shutdown();

	_selectReader.reset();
	_selectTarget.reset();

	_colorTarget.reset();
	_glowTarget.reset();
	_blurTarget[0].reset(); _blurTarget[1].reset();

	_clear.reset();
	_copy.reset();
	_blur.reset();
	_post.reset();
	_fxaa.reset();

	_renderContext = LUCID_GIGL::Context();
}

void Renderer::render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant, bool useFXAA)
{
	LUCID_PROFILE_SCOPE("Renderer::render(...)");
	LUCID_VALIDATE(nullptr != cameraFrame->focus, "camera does not have a focus specified");

	resize();

	_culler.cull(rootFrame, cameraFrame, interpolant);

	float32_t width = float32_t(_width);
	float32_t height = float32_t(_height);

	Frame *focusFrame = cameraFrame->focus;
	_interpolant = interpolant;

	_cameraPosition = LUCID_MATH::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
	_focusPosition = LUCID_MATH::lerp(interpolant, focusFrame->absolutePosition[0], focusFrame->absolutePosition[1]);

	LUCID_GAL::Matrix4x4 viewMatrix = LUCID_MATH::look(LUCID_GAL::Vector3(0, 0, 0), adaptiveScale(_focusPosition - _cameraPosition), LUCID_GAL::Vector3(0, 0, 1));
	LUCID_GAL::Matrix4x4 projMatrix = LUCID_MATH::perspective(cast(cameraFrame->fov), LUCID_GAL::Scalar(width / height), adaptiveScale(_culler.znear), adaptiveScale(_culler.zfar));
	LUCID_GAL::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

	_renderContext["screenWidth"] = LUCID_GAL::Scalar(width);
	_renderContext["screenHeight"] = LUCID_GAL::Scalar(height);
	_renderContext["texelSize"] = LUCID_GAL::Vector2(1.f / width, 1.f / height);

	_renderContext["time"] = cast(time);
	_renderContext["interpolant"] = cast(_interpolant);
		
	_renderContext["lightPosition"] = adaptiveScale(vector3_t(0, 0, 0) - _cameraPosition);

	_renderContext["viewPosition"] = LUCID_GAL::Vector3(0, 0, 0);
	_renderContext["viewForward"] = LUCID_MATH::extractViewForward(viewMatrix);
	_renderContext["viewRight"] = LUCID_MATH::extractViewRight(viewMatrix);
	_renderContext["viewUp"] = LUCID_MATH::extractViewUp(viewMatrix);
	_renderContext["viewMatrix"] = viewMatrix;
	_renderContext["invViewMatrix"] = LUCID_MATH::inverse(viewMatrix);
	_renderContext["projMatrix"] = projMatrix;
	_renderContext["invProjMatrix"] = LUCID_MATH::inverse(projMatrix);
	_renderContext["viewProjMatrix"] = viewProjMatrix;
	_renderContext["invViewProjMatrix"] = LUCID_MATH::inverse(viewProjMatrix);

	_sceneBatch.clear();
	_orbitBatch.clear();

	batch(rootFrame);

	render();

	copy  (_blurTarget[0].get(), _glowTarget.get());
	blur  ();
	copy  (_glowTarget.get(), _blurTarget[0].get());

	galPipeline().restoreBackBuffer(true, false, false);
	galPipeline().updateTargets();

	if (useFXAA)
		fxaaPost();
	else
		post();
}

uint32_t Renderer::hit(int32_t x, int32_t y) const
{
	x = LUCID_MATH::clamp(x, 0,  _width - 1);
	y = LUCID_MATH::clamp(y, 0, _height - 1);

	uint32_t const *select = (uint32_t const *)(_selectReader->read());
	return select[y * _width + x];
}

void Renderer::batch(Frame *frame)
{
	LUCID_PROFILE_SCOPE("Renderer::batch(Frame)");

	if (nullptr == frame)
		return;

	if (Culler::STATE_PRUNED == _culler[frame->id])
		return;

	if (Culler::STATE_VISIBLE == _culler[frame->id])
		frame->apply(this);

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		batch(child);
}

void Renderer::render()
{
	galPipeline().setRenderTarget(0, _colorTarget.get());
	galPipeline().setRenderTarget(1, _glowTarget.get());
	galPipeline().setRenderTarget(2, _selectTarget.get());

	galPipeline().updateTargets();

	_clear->render(_renderContext);

	renderStarfield();
	renderScene();
	renderOrbits();

	galPipeline().restoreBackBuffer(true, false, false);
	galPipeline().updateTargets();
}

void Renderer::renderStarfield()
{
	auto program = _starMesh->material()->program();

	program->set(_starParameters.sphereRadius, adaptiveScale(_culler.starFieldRadius));
	program->set(_starParameters. spriteScale, adaptiveScale(_culler.starScalingFactor));
		
	galPipeline().setVertexStream(1, _starInstances.get());
	_starMesh->renderInstanced(_renderContext, int32_t(_starCount));
}

void Renderer::renderScene()
{
	_sceneBatch.render(_renderContext);
}

void Renderer::renderOrbits()
{
	/// TBD: implement
}

void Renderer::copy(LUCID_GAL::RenderTarget2D *dst, LUCID_GAL::RenderTarget2D *src)
{
	galPipeline().setRenderTarget(0, dst);
	galPipeline().setRenderTarget(1, nullptr);
	galPipeline().setRenderTarget(2, nullptr);
	galPipeline().updateTargets();
		
	_copy->material()->program()->set(_copyParameters.theSource, src);
	_copy->render(_renderContext);
}

void Renderer::blur()
{
	float32_t width = float32_t(_width);
	float32_t height = float32_t(_height);

	LUCID_GAL::Vector2 horizontal = LUCID_GAL::Vector2(1.f / width, 0);
	LUCID_GAL::Vector2 vertical = LUCID_GAL::Vector2(0, 1.f / height);

	galPipeline().setRenderTarget(0, _blurTarget[1].get());
	galPipeline().updateTargets();

	_blur->material()->program()->set(_blurParameters.theSource, _blurTarget[0].get());
	_blur->material()->program()->set(_blurParameters.texelOffset, horizontal);
	_blur->render(_renderContext);

	galPipeline().setRenderTarget(0, _blurTarget[0].get());
	galPipeline().updateTargets();

	_blur->material()->program()->set(_blurParameters.theSource, _blurTarget[1].get());
	_blur->material()->program()->set(_blurParameters.texelOffset, vertical);
	_blur->render(_renderContext);
}

void Renderer::post()
{
	_post->material()->program()->set(_postParameters.colorTarget, _colorTarget.get());
	_post->material()->program()->set(_postParameters. glowTarget, _glowTarget. get());
	_post->render(_renderContext);
}

void Renderer::fxaaPost()
{
	_fxaa->material()->program()->set(_fxaaParameters.colorTarget, _colorTarget.get());
	_fxaa->material()->program()->set(_fxaaParameters.glowTarget, _glowTarget.get());
	_fxaa->render(_renderContext);
}

void Renderer::resize()
{
	LUCID_GAL::System &galSystem = LUCID_GAL::System::instance();
	if ((_width == galSystem.width()) && (_height == galSystem.height()))
		return;

	_width = galSystem.width();
	_height = galSystem.height();

	_selectTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
	_selectReader.reset(LUCID_GAL::TargetReader2D::create(_selectTarget.get(), _width, _height));

	_colorTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_glowTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[0].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[1].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
}

LUCID_ORBIT_END