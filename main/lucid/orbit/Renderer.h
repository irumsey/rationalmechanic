#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Profiler.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Algorithm.h>

namespace lucid {
namespace gal {

	class VertexBuffer;

}	///	gal
}	/// lucid

namespace lucid {
namespace gigl {

	class Context;
	class Mesh;

}	///	gigl
}	/// lucid

namespace lucid {
namespace orbit {

	class Frame;
	class DynamicPoint;
	class OrbitalBody;
	class DynamicBody;

	///
	///
	///
	class Renderer : public Algorithm
	{
	public:
		Renderer();

		virtual ~Renderer();

		virtual void evaluate(DynamicPoint *point) override;

		virtual void evaluate(OrbitalBody *body) override;

		virtual void evaluate(DynamicBody *body) override;

		void initialize();

		void shutdown();

		void render(Frame *root, ::lucid::gigl::Context const &context, float32_t time, float32_t interpolant);

	private:
		enum { BATCH_MAXIMUM = 100 };

		struct SphereInstance
		{
			::lucid::gal::Vector3 position;
			float32_t scale;
			::lucid::gal::Color color;
		};

		struct MaskInstance
		{
			::lucid::gal::Vector3 position;
			float32_t scale;
		};

		struct OrbitInstance
		{
			::lucid::gal::Vector4 parameters; // hu, e, theta0, theta1
			float32_t lineWidth;
			::lucid::gal::Color lineColor;
			::lucid::gal::Vector3 position;
			::lucid::gal::Quaternion rotation;
		};

		float32_t _time = 0.f;
		float32_t _interpolant = 0.f;

		std::vector<SphereInstance> _sphereBuffer;
		std::shared_ptr<lucid::gigl::Mesh> _sphereMesh;
		std::shared_ptr<lucid::gal::VertexBuffer> _sphereInstances;

		std::vector<MaskInstance> _maskBuffer;
		std::shared_ptr<lucid::gigl::Mesh> _maskMesh;
		std::shared_ptr<lucid::gal::VertexBuffer> _maskInstances;

		std::vector<OrbitInstance> _orbitBuffer;
		std::shared_ptr<lucid::gigl::Mesh> _orbitMesh;
		std::shared_ptr<lucid::gal::VertexBuffer> _orbitInstances;

		void batch(Frame *frame);

		void render(::lucid::gigl::Context const &context);

		LUCID_PREVENT_COPY(Renderer);
		LUCID_PREVENT_ASSIGNMENT(Renderer);
	};

	inline void Renderer::render(Frame *root, ::lucid::gigl::Context const &context, float32_t time, float32_t interpolant)
	{
		LUCID_PROFILE_BEGIN("rendering orbital objects");

		_time = time;
		_interpolant = interpolant;

		_sphereBuffer.clear();
		_maskBuffer.clear();
		_orbitBuffer.clear();

		batch(root);
		render(context);

		LUCID_PROFILE_END();
	}

}	///	orbit
}	///	lucid
