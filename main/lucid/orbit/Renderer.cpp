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

// test {
#include "Culler.h"
// } test

namespace  math = ::lucid:: math;
namespace   gal = ::lucid::  gal;
namespace  gigl = ::lucid:: gigl;
namespace orbit = ::lucid::orbit;

namespace { /// anonymous

	inline gal::Pipeline &galPipeline()
	{
		return gal::Pipeline::instance();
	}

	inline orbit::StarCatalog &theStarCatalog()
	{
		return orbit::StarCatalog::instance();
	}

	inline orbit::Ephemeris &theEphemeris()
	{
		return orbit::Ephemeris::instance();
	}

	template<typename T> struct NullSort
	{
		NullSort(gal::Vector3 const & /* dummy */)
		{
		}

		bool operator()(T const &, T const &) const
		{
			return false;
		}
	};

	template<typename T> struct Back2Front
	{
		gal::Vector3 viewPosition;

		Back2Front(gal::Vector3 const &viewPosition)
			: viewPosition(viewPosition)
		{
		}

		bool operator()(T const &lhs, T const &rhs) const
		{
			return math::lsq(lhs.position - viewPosition) > math::lsq(rhs.position - viewPosition);
		}
	};

	template<typename T> struct Front2Back
	{
		gal::Vector3 viewPosition;

		Front2Back(gal::Vector3 const &viewPosition)
			: viewPosition(viewPosition)
		{
		}

		bool operator()(T const &lhs, T const &rhs) const
		{
			return math::lsq(lhs.position - viewPosition) < math::lsq(rhs.position - viewPosition);
		}
	};

}	/// anonymous

namespace lucid {
namespace orbit {

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

		Frame const *centerFrame = body->centerFrame;
		gal::Vector3 centerPosition = adaptiveScale(math::lerp(_interpolant, centerFrame->absolutePosition[0], centerFrame->absolutePosition[1]) - _cameraPosition);
		gal::Vector3 bodyPosition = adaptiveScale(math::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition);

		gal::Scalar  a = adaptiveScale(math::lerp(_interpolant, elements[0].A, elements[1].A));
		gal::Scalar  e = cast(math::lerp(_interpolant, elements[0].EC, elements[1].EC));
		gal::Scalar hu = a * (1.f - e * e);

		gal::Quaternion rotation = math::slerp(
			cast(_interpolant).value,
			math::quaternionFromMatrix(cast(rotationFromElements(elements[0]))),
			math::quaternionFromMatrix(cast(rotationFromElements(elements[1])))
		);

		MeshInstance bodyInstance;
		bodyInstance.id = (SELECT_FRAME << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
		bodyInstance.position = bodyPosition;
		bodyInstance.scale = adaptiveScale(physicalProperties.radius);
		bodyInstance.rotation = gal::Quaternion(0, 0, 0, 1);
		bodyInstance.color = renderProperties.color;
		bodyInstance.parameters = renderProperties.parameters;
		_batched.addInstance(renderProperties.model, bodyInstance);

		if (!renderProperties.showOrbit)
			return;

		// test {
		// skip orbits for now.
		return;
		// } test

		MeshInstance orbitInstance;
		orbitInstance.id = (SELECT_ORBIT << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
		orbitInstance.position = centerPosition;
		orbitInstance.scale = 3.f;
		orbitInstance.rotation = rotation;
		orbitInstance.color = renderProperties.orbitHighlight ? gal::Color(1.f, 1.f, 1.f, 1.f) : gal::Color(0, 0, 1, 1);
		orbitInstance.parameters = gal::Vector4(hu, e, 0.f, constants::two_pi<float32_t>);
		_batched.addInstance(_orbitMesh, orbitInstance);
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

		_renderContext = gigl::Context(path);

		gal::System &galSystem = gal::System::instance();
		_width = galSystem.width();
		_height = galSystem.height();

		_starCount = theStarCatalog().count();
		_starMesh.reset(gigl::Mesh::create("content/star.mesh"));
		_starInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_STATIC, int32_t(_starCount), sizeof(StarInstance)));

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

		_batched.initialize();

		/// test {
		///	need a data driven method for registering these... 
		/// ...just read the ephemeris stupid!!!
		_batched.createBatch<MeshInstance, Back2Front<MeshInstance> >(gigl::Resources::get<gigl::Mesh>(    "content/sprite.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>(      "content/disk.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>("content/atmosphere.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>("content/hemisphere.mesh"), BATCH_MAXIMUM);
		/// } test
		 
		_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");
		_batched.createBatch<MeshInstance, Back2Front<MeshInstance> >(_orbitMesh, BATCH_MAXIMUM);

		_calloutMesh = gigl::Resources::get<gigl::Mesh>("content/callout.mesh");
		_batched.createBatch<CalloutInstance, NullSort<CalloutInstance> >(_calloutMesh, BATCH_MAXIMUM);

		_selectTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
		_selectReader.reset(gal::TargetReader2D::create(_selectTarget.get(), _width, _height));

		_colorTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_glowTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_blurTarget[0].reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_blurTarget[1].reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));

		_clear.reset(gigl::Mesh::create("content/clear.mesh"));
		_copy .reset(gigl::Mesh::create("content/copy.mesh"));
		_blur .reset(gigl::Mesh::create("content/blur.mesh"));
		_post .reset(gigl::Mesh::create("content/post.mesh"));
		_fxaa .reset(gigl::Mesh::create("content/fxaa.mesh"));

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

		_batched.shutdown();

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

		_renderContext = gigl::Context();
	}

	void Renderer::render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant, bool useFXAA)
	{
		LUCID_PROFILE_SCOPE("Renderer::render(...)");
		LUCID_VALIDATE(nullptr != cameraFrame->focus, "camera does not have a focus specified");

		resize();

		_interpolant = interpolant;
		_cameraPosition = math::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);

		// test {
		_culler.cull(rootFrame, cameraFrame, interpolant);
		// } test

		gal::Matrix4x4 viewMatrix = cast(_culler.viewMatrix);
		gal::Matrix4x4 projMatrix = cast(_culler.projMatrix);
		gal::Matrix4x4 viewProjMatrix = projMatrix * viewMatrix;
		gal::Matrix4x4 invViewProjMatrix = math::inverse(viewProjMatrix);

		float32_t width = float32_t(_width);
		float32_t height = float32_t(_height);

		_renderContext["screenWidth"] = gal::Scalar(width);
		_renderContext["screenHeight"] = gal::Scalar(height);
		_renderContext["znear"] = adaptiveScale(_culler.znear);
		_renderContext["zfar"] = adaptiveScale(_culler.zfar);
		_renderContext["texelSize"] = gal::Vector2(1.f / width, 1.f / height);

		_renderContext["time"] = cast(time);
		_renderContext["interpolant"] = cast(_interpolant);
		
		_renderContext["lightPosition"] = adaptiveScale(vector3_t(0, 0, 0) - _cameraPosition);

		_renderContext["viewPosition"] = gal::Vector3(0, 0, 0);
		_renderContext["viewForward"] = ::lucid::math::extractViewForward(viewMatrix);
		_renderContext["viewRight"] = ::lucid::math::extractViewRight(viewMatrix);
		_renderContext["viewUp"] = ::lucid::math::extractViewUp(viewMatrix);
		_renderContext["viewMatrix"] = viewMatrix;
		_renderContext["invViewMatrix"] = math::inverse(viewMatrix);
		_renderContext["projMatrix"] = projMatrix;
		_renderContext["invProjMatrix"] = math::inverse(projMatrix);
		_renderContext["viewProjMatrix"] = viewProjMatrix;
		_renderContext["invViewProjMatrix"] = invViewProjMatrix;

		_batched.clear();
		batch(rootFrame);

		render();

		copy  (_blurTarget[0].get(), _glowTarget.get());
		blur  ();
		copy  (_glowTarget.get(), _blurTarget[0].get());

		galPipeline().restoreBackBuffer(true, false, false);
		galPipeline().updateTargets();

		if (useFXAA)
			fxaa();
		else
			post();
	}

	uint32_t Renderer::hit(int32_t x, int32_t y) const
	{
		x = math::clamp(x, 0,  _width - 1);
		y = math::clamp(y, 0, _height - 1);

		uint32_t const *select = (uint32_t const *)(_selectReader->read());
		return select[y * _width + x];
	}

	void Renderer::batch(Frame *frame)
	{
		LUCID_PROFILE_SCOPE("Renderer::batch(Frame)");

		if (nullptr == frame)
			return;

		if (Culler::Visibility::STATE_PRUNED == _culler.visibility[frame->id].state)
			return;

		if (Culler::Visibility::STATE_VISIBLE == _culler.visibility[frame->id].state)
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

		/// test {
//		galPipeline().setVertexStream(1, _starInstances.get());
//		_starMesh->renderInstanced(_renderContext, int32_t(_starCount));
		/// } test

		_batched.render(_renderContext);
		
		galPipeline().restoreBackBuffer(true, false, false);
		galPipeline().updateTargets();
	}

	void Renderer::copy(gal::RenderTarget2D *dst, gal::RenderTarget2D *src)
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

		gal::Vector2 horizontal = gal::Vector2(1.f / width, 0);
		gal::Vector2 vertical = gal::Vector2(0, 1.f / height);

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

	void Renderer::fxaa()
	{
		_fxaa->material()->program()->set(_fxaaParameters.colorTarget, _colorTarget.get());
		_fxaa->material()->program()->set(_fxaaParameters.glowTarget, _glowTarget.get());
		_fxaa->render(_renderContext);
	}

	void Renderer::resize()
	{
		gal::System &galSystem = gal::System::instance();
		if ((_width == galSystem.width()) && (_height == galSystem.height()))
			return;

		_width = galSystem.width();
		_height = galSystem.height();

		_selectTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UINT_R32, _width, _height));
		_selectReader.reset(gal::TargetReader2D::create(_selectTarget.get(), _width, _height));

		_colorTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_glowTarget.reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_blurTarget[0].reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
		_blurTarget[1].reset(gal::RenderTarget2D::create(gal::RenderTarget2D::FORMAT_UNORM_R8G8B8A8, _width, _height));
	}

}	///	orbit 
}	///	lucid
