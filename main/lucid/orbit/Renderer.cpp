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

namespace  math = ::lucid:: math;
namespace   gal = ::lucid::  gal;
namespace  gigl = ::lucid:: gigl;
namespace orbit = ::lucid::orbit;

namespace { /// anonymous

	orbit::scalar_t const meters_per_ru = orbit::constants::meters_per_ru<orbit::scalar_t>();
	orbit::scalar_t const rus_per_meter = orbit::constants::rus_per_meter<orbit::scalar_t>();

	inline float32_t cast(orbit::scalar_t rhs)
	{
		return float32_t(rhs);
	}

	inline float32_t scale(orbit::scalar_t rhs)
	{
		return cast(rhs * rus_per_meter);
	}

	inline gal::Vector3 cast(orbit::vector3_t const &rhs)
	{
		return gal::Vector3(cast(rhs.x), cast(rhs.y), cast(rhs.z));
	}

	inline gal::Vector3 scale(orbit::vector3_t const &rhs)
	{
		return cast(rhs * rus_per_meter);
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
		///	TBD: use interpolant to interpolate between position0 and position1
		///	TBD: use interpolant to interpolate between rotation0 and rotation1
		///	TBD: use interpolant to interpolate between elements0 and elements1
		///	TBD: data drive the sphere's scale and color
		///	TBD: data drive line width and color
		///	TBD: data drive domain of orbit
		///	TBD: note not all orbital bodies will be spheres (they might be cool spacecraft)

		Elements const &elements = body->elements[1];

		float32_t  a = scale(elements. A);
		float32_t  e = cast (elements.EC);
		float32_t hu = a * (1.f - e * e);

		SphereInstance sphere;
		sphere.position = scale(body->absolutePosition[1]);
		sphere.scale = 5.f;
		sphere.color = gal::Color(0, 1, 0, 1);
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
		orbit.rotation = math::quaternionFromMatrix(cast(rotationFromElements(elements)));
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
		batchedRender(context, _sphereBuffer, _sphereMesh.get(), _sphereInstances.get(), size_t(BATCH_MAXIMUM));
		batchedRender(context,   _maskBuffer,   _maskMesh.get(),   _maskInstances.get(), size_t(BATCH_MAXIMUM));
		batchedRender(context,  _orbitBuffer,  _orbitMesh.get(),  _orbitInstances.get(), size_t(BATCH_MAXIMUM));
	}

}	///	orbit
}	///	lucid
