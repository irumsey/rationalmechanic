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

		PhysicalProperties physicalProperties;
		theEphemeris().lookup(physicalProperties, body->id);

		RenderProperties renderProperties;
		theEphemeris().lookup(renderProperties, body->id);

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

		SphereInstance sphere;
		sphere.position = math::lerp(_interpolant, position[0], position[1]);
		sphere.scale = renderProperties.scale * scale(physicalProperties.radius);
		sphere.color = renderProperties.color;
		_batched.addInstance(renderProperties.mesh, sphere);

		MaskInstance mask;
		mask.position = sphere.position;
		mask.scale = sphere.scale;
		_batched.addInstance(_orbitMask, mask);

		OrbitInstance orbit;
		orbit.parameters = gal::Vector4(hu, e, -3.1415926f, 3.1415926f);
		orbit.lineColor = gal::Color(0, 0, 1, 1);
		orbit.lineWidth = 0.5f;
		orbit.position = sphere.position;
		orbit.rotation = math::slerp(_interpolant, rotation[0], rotation[1]);
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

		_batched.createBatch<SphereInstance>(gigl::Resources::get<gigl::Mesh>(     "content/star.mesh"), BATCH_MAXIMUM);
		_batched.createBatch<SphereInstance>(gigl::Resources::get<gigl::Mesh>(   "content/sphere.mesh"), BATCH_MAXIMUM);

		_orbitMask = gigl::Resources::get<gigl::Mesh>("content/orbitMask.mesh");
		_batched.createBatch<  MaskInstance>(_orbitMask, BATCH_MAXIMUM);

		_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");
		_batched.createBatch< OrbitInstance>(_orbitMesh, BATCH_MAXIMUM);
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
