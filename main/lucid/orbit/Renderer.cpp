#include "Renderer.h"
#include "Ephemeris.h"
#include "Frame.h"
#include "Utility.h"
#include "Constants.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Resources.h>
#include <lucid/math/Scalar.h>
#include <algorithm>

namespace  math = ::lucid:: math;
namespace   gal = ::lucid::  gal;
namespace  gigl = ::lucid:: gigl;
namespace orbit = ::lucid::orbit;

namespace { /// anonymous

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
		///	TBD: implement
	}

	void Renderer::evaluate(OrbitalBody *body)
	{
		///	TBD: data drive line width and color
		///	TBD: data drive domain of orbit

		if (cull(body))
			return;

		PhysicalProperties const &physicalProperties = body->physicalProperties;
		RenderProperties &renderProperties = body->renderProperties;
		Elements const *elements = body->elements;

		float32_t  a = math::lerp(_interpolant, scale(elements[0]. A), scale(elements[1]. A));
		float32_t  e = math::lerp(_interpolant, cast (elements[0].EC), cast (elements[1].EC));
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
		size_t detailIndex = detailLevels.level(math::len(position[1] - _viewPosition));

		if (DetailLevels::INVALID_LEVEL == detailIndex)
			return;

		DetailLevels::Level const &detailLevel = detailLevels[detailIndex];

		MeshInstance sphere;
		sphere.position = math::lerp(_interpolant, position[0], position[1]);
		sphere.scale = detailLevel.scale * scale(physicalProperties.radius);
		sphere.rotation = gal::Quaternion(0, 0, 0, 1);
		sphere.color = detailLevel.color;
		sphere.parameters = detailLevel.parameters;
		_batched.addInstance(detailLevel.mesh, sphere);
		_batched.addInstance(_orbitMask, sphere);

		MeshInstance orbit;
		orbit.position = sphere.position;
		orbit.scale = 0.5f;
		orbit.rotation = math::slerp(_interpolant, rotation[0], rotation[1]);
		orbit.color = gal::Color(0, 0, 1, 1);
		orbit.parameters = gal::Vector4(hu, e, -3.1415926f, 3.1415926f);
		_batched.addInstance(_orbitMesh, orbit);
	}

	void Renderer::evaluate(DynamicBody *body)
	{
		///	TBD: implement
	}

	void Renderer::initialize()
	{
		shutdown();
		_batched.initialize();

		_batched.createBatch<MeshInstance, Back2Front<MeshInstance> >(gigl::Resources::get<gigl::Mesh>(   "content/sphere.mesh"), BATCH_MAXIMUM);

		_orbitMask = gigl::Resources::get<gigl::Mesh>("content/orbitMask.mesh");
		_batched.createBatch<MeshInstance,   NullSort<MeshInstance> >(_orbitMask, BATCH_MAXIMUM);

		_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");
		_batched.createBatch<MeshInstance,    NullSort<MeshInstance> >(_orbitMesh, BATCH_MAXIMUM);
	}

	void Renderer::shutdown()
	{
		_batched.shutdown();
	}

	void Renderer::render(Frame *root, ::lucid::gigl::Context const &context, float32_t time, float32_t interpolant)
	{
		LUCID_PROFILE_BEGIN("rendering orbital objects");

		_time = time;
		_interpolant = interpolant;

		_viewPosition = context.lookup("viewPosition").as<gal::Vector3>();
		_viewProjMatrix = context.lookup("viewProjMatrix").as<gal::Matrix4x4>();

		_batched.clear();
			batch(root);
		_batched.render(context);

		LUCID_PROFILE_END();
	}

	void Renderer::batch(Frame *frame)
	{
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
	}

}	///	orbit
}	///	lucid
