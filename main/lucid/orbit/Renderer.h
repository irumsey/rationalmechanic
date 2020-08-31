#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Profiler.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Algorithm.h>

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
			::lucid::gal::Vector4 parameters; // hu, e, theta_0, theta_1
			::lucid::gal::Vector3 position;
			::lucid::gal::Quaternion rotation;
			::lucid::gal::Color lineColor;
			float32_t lineWidth;
		};

		std::shared_ptr<gigl::Mesh> _orbitMask;
		std::shared_ptr<gigl::Mesh> _orbitMesh;

		float32_t _time = 0.f;
		float32_t _interpolant = 0.f;

		::lucid::gal::Vector3 _viewPosition;
		::lucid::gal::Matrix4x4 _viewProjMatrix;

		::lucid::gigl::Batched _batched;

		void batch(Frame *frame);

		bool cull(Frame *frame);

		LUCID_PREVENT_COPY(Renderer);
		LUCID_PREVENT_ASSIGNMENT(Renderer);
	};

}	///	orbit
}	///	lucid
