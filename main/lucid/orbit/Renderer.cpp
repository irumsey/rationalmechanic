#include "Renderer.h"
#include "Ephemeris.h"
#include "Frame.h"
#include "Utility.h"
#include "Constants.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/Pipeline.h>
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

	orbit::scalar_t const meters_per_RU = orbit::constants::meters_per_RU<orbit::scalar_t>();
	orbit::scalar_t const RUs_per_meter = orbit::constants::RUs_per_meter<orbit::scalar_t>();

	inline float32_t cast(orbit::scalar_t rhs)
	{
		return float32_t(rhs);
	}

	inline float32_t scale(orbit::scalar_t rhs)
	{
		return cast(rhs * RUs_per_meter);
	}

	inline gal::Vector3 cast(orbit::vector3_t const &rhs)
	{
		return gal::Vector3(cast(rhs.x), cast(rhs.y), cast(rhs.z));
	}

	inline gal::Vector3 scale(orbit::vector3_t const &rhs)
	{
		return cast(rhs * RUs_per_meter);
	}

	inline gal::Matrix3x3 cast(orbit::matrix3x3_t const &rhs)
	{
		return gal::Matrix3x3(
			cast(rhs.xx), cast(rhs.xy), cast(rhs.xz),
			cast(rhs.yx), cast(rhs.yy), cast(rhs.yz),
			cast(rhs.zx), cast(rhs.zy), cast(rhs.zz)
		);
	}

	template<typename I> void batchedRender(gigl::Context const &context, std::vector<I> const &buffer, gigl::Mesh *mesh, gal::VertexBuffer *batch, size_t batchMaximum)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		size_t totalCount = buffer.size();
		size_t index = 0;

		std::shared_ptr<gigl::Material> material = mesh->material();

		while (index < totalCount)
		{
			size_t count = math::min(totalCount - index, batchMaximum);

			I *instances = (I*)batch->lock();
				::memcpy(instances, &buffer[index], count * sizeof(I));
			batch->unlock();

			material->begin(context);
			pipeline.setVertexStream(1, batch);
				mesh->drawInstanced(count);
			material->end();

			index += count;
		}
	}

}	/// anonymous

namespace lucid {
namespace orbit {

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
		///	TBD: note not all orbital bodies will be spheres (they might be cool spacecraft)

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
		_sphereBuffer.push_back(sphere);

		MaskInstance mask;
		mask.position = sphere.position;
		mask.scale = sphere.scale;
		_maskBuffer.push_back(mask);

		OrbitInstance orbit;
		orbit.parameters = gal::Vector4(hu, e, -3.1415926f, 3.1415926f);
		orbit.lineColor = gal::Color(0, 0, 1, 1);
		orbit.lineWidth = 0.5f;
		orbit.position = sphere.position;
		orbit.rotation = math::slerp(_interpolant, rotation[0], rotation[1]);
		_orbitBuffer.push_back(orbit);
	}

	void Renderer::evaluate(DynamicBody *body)
	{
		///	TBD: implement
	}

	void Renderer::initialize()
	{
		shutdown();

		_sphereMesh = gigl::Resources::get<gigl::Mesh>("content/sphere.mesh");
		_sphereInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, BATCH_MAXIMUM, sizeof(SphereInstance)));

		_maskMesh = gigl::Resources::get<gigl::Mesh>("content/orbitMask.mesh");
		_maskInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, BATCH_MAXIMUM, sizeof(MaskInstance)));

		_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");
		_orbitInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, BATCH_MAXIMUM, sizeof(OrbitInstance)));
	}

	void Renderer::shutdown()
	{
		_sphereMesh.reset();
		_sphereInstances.reset();

		_maskMesh.reset();
		_maskInstances.reset();

		_orbitMesh.reset();
		_orbitInstances.reset();
	}

	void Renderer::render(Frame *root, ::lucid::gigl::Context const &context, float32_t time, float32_t interpolant)
	{
		LUCID_PROFILE_BEGIN("rendering orbital objects");

		_time = time;
		_interpolant = interpolant;

		_viewPosition = context.lookup("viewPosition").as<gal::Vector3>();
		_viewProjMatrix = context.lookup("viewProjMatrix").as<gal::Matrix4x4>();

		_sphereBuffer.clear();
		_maskBuffer.clear();
		_orbitBuffer.clear();

		batch(root);
		render(context);

		LUCID_PROFILE_END();
	}

	bool Renderer::cull(Frame *frame) const
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

	void Renderer::render(gigl::Context const &context)
	{
		/// test {
		///	sort spheres back to front (for alpha blending)
		gal::Vector3 viewPosition = context.lookup("viewPosition").as<gal::Vector3>();
		struct Predicate
		{
			gal::Vector3 viewPosition;

			Predicate(gal::Vector3 const &viewPosition)
				: viewPosition(viewPosition)
			{
			}

			inline bool operator()(SphereInstance const &lhs, SphereInstance const &rhs) const
			{
				gal::Vector3 a = lhs.position - viewPosition;
				gal::Vector3 b = rhs.position - viewPosition;

				return math::lsq(a) > math::lsq(b);
			}
		};
		std::sort(_sphereBuffer.begin(), _sphereBuffer.end(), Predicate(viewPosition));
		/// } test

		batchedRender(context, _sphereBuffer, _sphereMesh.get(), _sphereInstances.get(), size_t(BATCH_MAXIMUM));
		batchedRender(context,   _maskBuffer,   _maskMesh.get(),   _maskInstances.get(), size_t(BATCH_MAXIMUM));
		batchedRender(context,  _orbitBuffer,  _orbitMesh.get(),  _orbitInstances.get(), size_t(BATCH_MAXIMUM));
	}

}	///	orbit
}	///	lucid
