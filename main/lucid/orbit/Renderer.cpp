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
#include <lucid/math/Algorithm.h>
#include <algorithm>

#include <lucid/gigl/Font.h>

// give these macros a try, see if i like them...
#define GET_MATERIAL_PARAMETER(mesh, name) mesh->material()->program()->lookup( #name )
#define SET_MATERIAL_PARAMETER(mesh, param, value) mesh->material()->program()->set(param, value)

LUCID_ANONYMOUS_BEGIN

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
	bool operator()(T const &, T const &) const
	{
		return false;
	}
};

template<typename T> struct Back2Front
{
	bool operator()(T const &lhs, T const &rhs) const
	{
		return lhs.distance > rhs.distance;
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

void Renderer::initialize(std::string const &path)
{
	shutdown();

	_renderContext = LUCID_GIGL::Context(path);

	_width = LUCID_GAL_SYSTEM.width();
	_height = LUCID_GAL_SYSTEM.height();

	_screenSize = LUCID_GAL::Vector2(float32_t(_width), float32_t(_height));
	_texelSize = LUCID_GAL::Vector2(1.f / _screenSize.x, 1.f / _screenSize.y);

	_compositor.initialize(BATCH_MAXIMUM, _zmid);
	_iconBatch.initialize();

	_starCount = theStarCatalog().count();
	_starMesh.reset(LUCID_GIGL::Mesh::create("content/star.mesh"));
	_starInstances.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, int32_t(_starCount), sizeof(StarInstance)));

	StarInstance *starInstances = (StarInstance *)(_starInstances->lock());
	for (size_t i = 0; i < _starCount; ++i)
	{
		StarCatalog::Entry const &entry = theStarCatalog()[i];
		StarInstance &instance = starInstances[i];

		instance.          id = uint32_t((Selection::TYPE_STAR << Selection::SELECT_SHIFT) | i);
		instance.parameters.x = /* unused */ 0.f;
		instance.parameters.y = float32_t(entry.right_ascension);
		instance.parameters.z = float32_t(entry.declination);
		instance.parameters.w = 0.2f * entry.magnitude;
		instance.       color = entry.color;
	}
	_starInstances->unlock();

	/// test {
	///	need a data driven method for registering these... 
	/// ...just read the ephemeris stupid!!!
	_iconBatch.createBatch<IconInstance, NullSort<IconInstance> >(LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(  "content/iconDefault.mesh"), BATCH_MAXIMUM, NullSort<IconInstance>());
	_iconBatch.createBatch<IconInstance, NullSort<IconInstance> >(LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>("content/iconSatellite.mesh"), BATCH_MAXIMUM, NullSort<IconInstance>());
	/// } test

	_font = LUCID_GIGL::Resources::get<LUCID_GIGL::Font>("content/OCRa.font");
	_text.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, TEXT_LENGTH_MAXIMUM, sizeof(LUCID_GIGL::Font::Character)));

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

	_starParameters.sphereRadius = GET_MATERIAL_PARAMETER(_starMesh, sphereRadius);
	_starParameters. spriteScale = GET_MATERIAL_PARAMETER(_starMesh, spriteScale);  
	_copyParameters.   theSource = GET_MATERIAL_PARAMETER(    _copy,    theSource);
	_blurParameters. texelOffset = GET_MATERIAL_PARAMETER(    _blur,  texelOffset);
	_blurParameters.   theSource = GET_MATERIAL_PARAMETER(    _blur,    theSource);
	_postParameters. colorTarget = GET_MATERIAL_PARAMETER(    _post,  colorTarget);
	_postParameters.  glowTarget = GET_MATERIAL_PARAMETER(    _post,   glowTarget);
	_fxaaParameters. colorTarget = GET_MATERIAL_PARAMETER(    _fxaa,  colorTarget);
	_fxaaParameters.  glowTarget = GET_MATERIAL_PARAMETER(    _fxaa,   glowTarget);
}

void Renderer::shutdown()
{
	_starCount = 0;
	_starInstances.reset();
	_starMesh.reset();

	_text.reset();
	_font.reset();

	_iconBatch.shutdown();

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
	_compositor.process(rootFrame, cameraFrame, interpolant);

	Frame *focusFrame = cameraFrame->focus;
	_interpolant = interpolant;

	float32_t fov = cast(cameraFrame->fov);

	_cameraPosition = LUCID_MATH::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
	_focusPosition = LUCID_MATH::lerp(interpolant, focusFrame->absolutePosition[0], focusFrame->absolutePosition[1]);

	_renderContext["screenSize"] = _screenSize;
	_renderContext["texelSize"] = _texelSize;

	_renderContext["time"] = cast(time);
	_renderContext["interpolant"] = cast(_interpolant);
		
	_renderContext["lightDirection"] = LUCID_MATH::normalize(cast(vector3_t(0, 0, 0) - _cameraPosition));

	LUCID_GAL::Matrix4x4 viewMatrix = LUCID_MATH::look(LUCID_GAL::Vector3(0, 0, 0), cast(_focusPosition - _cameraPosition), LUCID_GAL::Vector3(0, 0, 1));
	LUCID_GAL::Matrix4x4 projMatrix = LUCID_MATH::perspective(fov, _aspect, _znear, _zfar);
	LUCID_GAL::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;

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

	_iconBatch.clear();

	preRender();
	render();
	postRender(useFXAA);
}

Selection Renderer::hit(int32_t x, int32_t y) const
{
	x = LUCID_MATH::clamp(x, 0,  _width - 1);
	y = LUCID_MATH::clamp(y, 0, _height - 1);

	uint32_t const *target = (uint32_t const *)(_selectReader->read());
	uint32_t code = target[y * _width + x];

	Selection selection;
	selection.type = (Selection::TYPE)((0xf0000000 & code) >> Selection::SELECT_SHIFT);
	selection.token = (Selection::SELECT_MASK & code);

	return selection;
}

#if false

void Renderer::batchOrbit(OrbitalBody *body)
{
	RenderProperties const &renderProperties = body->renderProperties;
	if (!renderProperties.showOrbit)
		return;

	PhysicalProperties const &physicalProperties = body->physicalProperties;
	Elements const *elements = body->elements;

	Frame const *centerFrame = body->centerFrame;
	vector3_t centerPosition = LUCID_MATH::lerp(_interpolant, centerFrame->absolutePosition[0], centerFrame->absolutePosition[1]) - _cameraPosition;
	scalar_t centerDistance = LUCID_MATH::len(centerPosition);
	scalar_t d1 = 100.0;

	quaternion_t q0 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(elements[0]));
	quaternion_t q1 = LUCID_MATH::quaternionFromMatrix(rotationFromElements(elements[1]));
	quaternion_t  q = LUCID_MATH::slerp(_interpolant, q0, q1);

	float32_t  a = cast(d1 * LUCID_MATH::lerp(_interpolant, elements[0].A, elements[1].A) / centerDistance);
	float32_t  e = cast(LUCID_MATH::lerp(_interpolant, elements[0].EC, elements[1].EC));
	float32_t hu = a * (1.f - e * e);

	MeshInstance orbitInstance;
	orbitInstance.id = (SELECT_ORBIT << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
	orbitInstance.position = cast(d1 * centerPosition / centerDistance);
	orbitInstance.scale = 4;
	orbitInstance.rotation = cast(q);
	orbitInstance.color = renderProperties.orbitHighlight ? LUCID_GAL::Color(1.f, 1.f, 1.f, 1.f) : LUCID_GAL::Color(0, 0, 1, 1);
	orbitInstance.parameters = LUCID_GAL::Vector4(hu, e, /* from */ 0, /* to */ constants::two_pi<float32_t>);
	_orbitBatch.addInstance(_orbitMesh, orbitInstance);
}

#endif

void Renderer::preRender()
{
	LUCID_GAL_PIPELINE.setRenderTarget(0, _colorTarget.get());
	LUCID_GAL_PIPELINE.setRenderTarget(1, _glowTarget.get());
	LUCID_GAL_PIPELINE.setRenderTarget(2, _selectTarget.get());

	LUCID_GAL_PIPELINE.updateTargets();

	_clear->render(_renderContext);
}

void Renderer::render()
{
	SET_MATERIAL_PARAMETER(_starMesh, _starParameters.sphereRadius, 0.999f * _zfar);
	SET_MATERIAL_PARAMETER(_starMesh, _starParameters.spriteScale, 1.2f);

	LUCID_GAL_PIPELINE.setVertexStream(1, _starInstances.get());
	_starMesh->renderInstanced(_renderContext, int32_t(_starCount));

	_compositor.render(_renderContext);
	_iconBatch.render(_renderContext);

	if (0 == _textCount)
		return;

	LUCID_GAL_PIPELINE.setVertexStream(1, _text.get());
	_font->renderInstanced(_renderContext, _textCount);
}

void Renderer::postRender(bool useFXAA)
{
	LUCID_GAL_PIPELINE.restoreBackBuffer(true, false, false);
	LUCID_GAL_PIPELINE.updateTargets();

	copy(_blurTarget[0].get(), _glowTarget.get());
	blur();
	blur();
	blur();
	copy(_glowTarget.get(), _blurTarget[0].get());

	LUCID_GAL_PIPELINE.restoreBackBuffer(true, false, false);
	LUCID_GAL_PIPELINE.updateTargets();

	if (useFXAA)
	{
		SET_MATERIAL_PARAMETER(_fxaa, _fxaaParameters.colorTarget, _colorTarget.get());
		SET_MATERIAL_PARAMETER(_fxaa, _fxaaParameters.glowTarget, _glowTarget.get());
		_fxaa->render(_renderContext);
	}
	else
	{
		SET_MATERIAL_PARAMETER(_post, _postParameters.colorTarget, _colorTarget.get());
		SET_MATERIAL_PARAMETER(_post, _postParameters.glowTarget, _glowTarget.get());
		_post->render(_renderContext);
	}
}

void Renderer::copy(LUCID_GAL::RenderTarget2D *dst, LUCID_GAL::RenderTarget2D *src)
{
	LUCID_GAL_PIPELINE.setRenderTarget(0, dst);
	LUCID_GAL_PIPELINE.setRenderTarget(1, nullptr);
	LUCID_GAL_PIPELINE.setRenderTarget(2, nullptr);
	LUCID_GAL_PIPELINE.updateTargets();
	
	SET_MATERIAL_PARAMETER(_copy, _copyParameters.theSource, src);
	_copy->render(_renderContext);
}

void Renderer::blur()
{
	LUCID_GAL::Vector2 horizontal = LUCID_GAL::Vector2(_texelSize.x,          0.f);
	LUCID_GAL::Vector2   vertical = LUCID_GAL::Vector2(         0.f, _texelSize.y);

	LUCID_GAL_PIPELINE.setRenderTarget(0, _blurTarget[1].get());
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(_blur, _blurParameters.theSource, _blurTarget[0].get());
	SET_MATERIAL_PARAMETER(_blur, _blurParameters.texelOffset, horizontal);
	_blur->render(_renderContext);

	LUCID_GAL_PIPELINE.setRenderTarget(0, _blurTarget[0].get());
	LUCID_GAL_PIPELINE.updateTargets();

	SET_MATERIAL_PARAMETER(_blur, _blurParameters.theSource, _blurTarget[1].get());
	SET_MATERIAL_PARAMETER(_blur, _blurParameters.texelOffset, vertical);
	_blur->render(_renderContext);
}

void Renderer::resize()
{
	if ((_width == LUCID_GAL_SYSTEM.width()) && (_height == LUCID_GAL_SYSTEM.height()))
		return;

	_width = LUCID_GAL_SYSTEM.width();
	_height = LUCID_GAL_SYSTEM.height();
	_aspect = LUCID_GAL_SYSTEM.aspect();

	_screenSize = LUCID_GAL::Vector2(float32_t(_width), float32_t(_height));
	_texelSize = LUCID_GAL::Vector2(1.f / _screenSize.x, 1.f / _screenSize.y);

	_selectTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
	_selectReader.reset(LUCID_GAL::TargetReader2D::create(_selectTarget.get(), _width, _height));

	_colorTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_glowTarget.reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[0].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	_blurTarget[1].reset(LUCID_GAL::RenderTarget2D::create(LUCID_GAL::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
}

LUCID_ORBIT_END