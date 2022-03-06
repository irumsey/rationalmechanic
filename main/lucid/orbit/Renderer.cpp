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
#include <algorithm>

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

		if (cull(body))
			return;

		Frame const *center = body->centerFrame;
		gal::Vector3 centerPosition = math::lerp(_interpolant, scale(center->absolutePosition[0]), scale(center->absolutePosition[1]));

		PhysicalProperties const &physicalProperties = body->physicalProperties;
		RenderProperties &renderProperties = body->renderProperties;
		Elements const *elements = body->elements;

		float32_t  a = math::lerp(_interpolant, scale(elements[0].A), scale(elements[1].A));
		float32_t  e = math::lerp(_interpolant, cast(elements[0].EC), cast(elements[1].EC));
		float32_t hu = a * (1.f - e * e);

		gal::Quaternion rotation[2] = {
			math::quaternionFromMatrix(cast(rotationFromElements(elements[0]))),
			math::quaternionFromMatrix(cast(rotationFromElements(elements[1])))
		};

		gal::Vector3 position[2] = {
			scale(body->absolutePosition[0]),
			scale(body->absolutePosition[1]),
		};

		DetailLevels &detailLevels = renderProperties.detailLevels;

		size_t detailIndex = detailLevels.level(math::len(position[1] - _cameraPosition));
		if (DetailLevels::INVALID_LEVEL == detailIndex)
			return;

		DetailLevels::Level const &detailLevel = detailLevels[detailIndex];

		MeshInstance bodyInstance;
		bodyInstance.id = (SELECT_FRAME << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
		bodyInstance.position = math::lerp(_interpolant, position[0], position[1]);
		bodyInstance.scale = detailLevel.scale * scale(physicalProperties.radius);
		bodyInstance.rotation = gal::Quaternion(0, 0, 0, 1);
		bodyInstance.color = detailLevel.color;
		bodyInstance.parameters = detailLevel.parameters;
		_batched.addInstance(detailLevel.model, bodyInstance);

		MeshInstance orbitInstance;
		orbitInstance.id = (SELECT_ORBIT << SELECT_SHIFT) | uint32_t(SELECT_MASK & body->id);
		orbitInstance.position = centerPosition;
		orbitInstance.scale = 0.2f;
		orbitInstance.rotation = math::slerp(_interpolant, rotation[0], rotation[1]);
		orbitInstance.color = gal::Color(0, 0, 1, 1);
		orbitInstance.parameters = gal::Vector4(hu, e, 0.f, math::constants::two_pi<float32_t>());
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

			instance.parameters.x = 0.3f;
			instance.parameters.y = orbit::cast(entry.right_ascension);
			instance.parameters.z = orbit::cast(entry.declination);
			instance.parameters.w = entry.magnitude;
		}
		_starInstances->unlock();

		_batched.initialize();

		/// test {
		///	need a data driven method for registering these... 
		/// ...just read the ephemeris stupid!!!
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>(      "content/disk.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>("content/atmosphere.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<MeshInstance, Front2Back<MeshInstance> >(gigl::Resources::get<gigl::Mesh>("content/hemisphere.mesh"), BATCH_MAXIMUM);
		/// } test

		_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");
		_batched.createBatch<MeshInstance, Back2Front<MeshInstance> >(_orbitMesh, BATCH_MAXIMUM);

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

		_copyParameters.  theSource = _copy->material()->program()->lookup(  "theSource");
		_blurParameters.  texelSize = _blur->material()->program()->lookup(  "texelSize");
		_blurParameters.  theSource = _blur->material()->program()->lookup(  "theSource");
		_postParameters.colorTarget = _post->material()->program()->lookup("colorTarget");
		_postParameters. glowTarget = _post->material()->program()->lookup( "glowTarget");
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

		_renderContext = gigl::Context();
	}

	void Renderer::render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant)
	{
		LUCID_PROFILE_SCOPE("Renderer::render(...)");

		resize();

		gigl::Camera2D &camera = cameraFrame->camera;

		_time = cast(time);
		_interpolant = cast(interpolant);
		_cameraPosition = camera.getPosition();

		Frame const *focusFrame = cameraFrame->focus;

		camera.look(
			scale(math::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1])),
			scale(math::lerp(interpolant, focusFrame->absolutePosition[0], focusFrame->absolutePosition[1])),
			gal::Vector3(0, 0, 1)
		);

		_renderContext["time"] = _time;
		_renderContext["interpolant"] = _interpolant;
		_renderContext["viewPosition"] = camera.getPosition();
		_renderContext["viewForward"] = camera.getForward();
		_renderContext["viewRight"] = camera.getRight();
		_renderContext["viewUp"] = camera.getUp();
		_renderContext["viewMatrix"] = camera.getViewMatrix();
		_renderContext["invViewMatrix"] = math::inverse(camera.getViewMatrix());
		_renderContext["projMatrix"] = camera.getProjMatrix();
		_renderContext["invProjMatrix"] = math::inverse(camera.getProjMatrix());
		_renderContext["viewProjMatrix"] = camera.getViewProjMatrix();
		_renderContext["invViewProjMatrix"] = math::inverse(camera.getViewProjMatrix());

		_batched.clear();
		batch(rootFrame);

		render();
		copy  (_blurTarget[0].get(), _glowTarget.get());
		blur  ();
		copy  (_glowTarget.get(), _blurTarget[0].get());

		galPipeline().restoreBackBuffer(true, false, false);
		galPipeline().updateTargets();

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

		frame->apply(this);
		for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		{
			batch(child);
		}
	}

	bool Renderer::cull(Frame *frame)
	{
		LUCID_PROFILE_SCOPE("Renderer::cull(Frame)");

#if false
		///	TBD: more sophistication
		///	for now, just based upon distance to center body

		///	(32 pixels / 1024 pixels)^2
		///	so, if the screen is 1024 pixels wide, anything less then 32ish pixels will be culled
		float32_t const magic = math::pow(32.f / 1024.f, 2.f);

		Frame *center = frame->centerFrame;
		if (nullptr == center)
			return true;

		///	don't cull the sun
		if (10 == frame->id)
			return false;

		///	measuring the distance from the target frame to its attracting center

		gal::Vector4 ppsPosition[2] = {
			_viewProjMatrix * gal::Vector4(scale(center->absolutePosition[1]), 1.f),
			_viewProjMatrix * gal::Vector4(scale(frame->absolutePosition[1]), 1.f),
		};

		gal::Vector2 screenPosition[2] = {
			gal::Vector2(ppsPosition[0].x, ppsPosition[0].y) / ppsPosition[0].w,
			gal::Vector2(ppsPosition[1].x, ppsPosition[1].y) / ppsPosition[1].w,
		};

		return (math::lsq(screenPosition[1] - screenPosition[0]) < 0.004f); 
#endif

		return false;
	}

	void Renderer::render()
	{
		galPipeline().setRenderTarget(0, _colorTarget.get());
		galPipeline().setRenderTarget(1, _glowTarget.get());
		galPipeline().setRenderTarget(2, _selectTarget.get());

		galPipeline().updateTargets();

		_clear->render(_renderContext);

		galPipeline().setVertexStream(1, _starInstances.get());
		_starMesh->renderInstanced(_renderContext, int32_t(_starCount));

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
		gal::Vector2 horizontal = gal::Vector2(1.f / _width, 0);
		gal::Vector2 vertical = gal::Vector2(0, 1.f / _height);

		galPipeline().setRenderTarget(0, _blurTarget[1].get());
		galPipeline().updateTargets();

		_blur->material()->program()->set(_blurParameters.theSource, _blurTarget[0].get());
		_blur->material()->program()->set(_blurParameters.texelSize, horizontal);
		_blur->render(_renderContext);

		galPipeline().setRenderTarget(0, _blurTarget[0].get());
		galPipeline().updateTargets();

		_blur->material()->program()->set(_blurParameters.theSource, _blurTarget[1].get());
		_blur->material()->program()->set(_blurParameters.texelSize, vertical);
		_blur->render(_renderContext);
	}

	void Renderer::post()
	{
		_post->material()->program()->set(_postParameters.colorTarget, _colorTarget.get());
		_post->material()->program()->set(_postParameters. glowTarget, _glowTarget. get());
		_post->render(_renderContext);
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
