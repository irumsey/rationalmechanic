#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Profiler.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Algorithm.h>

namespace lucid {
namespace gal {

	class Parameter;
	class VertexBuffer;
	class RenderTarget2D;
	class TargetReader2D;

}	///	gal
}	///	lucid

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
		enum SELECT
		{
			SELECT_STAR  = 0x01,
			SELECT_FRAME = 0x02,
			SELECT_ORBIT = 0x04,
			SELECT_OTHER = 0x08,
		};
		enum { SELECT_SHIFT = 28 };
		enum { SELECT_MASK  = 0x0fffffff};
		Renderer();

		virtual ~Renderer();

		virtual void evaluate(DynamicPoint *point) override;

		virtual void evaluate(OrbitalBody *body) override;

		virtual void evaluate(DynamicBody *body) override;

		void initialize();

		void shutdown();

		void render(Frame *root, gigl::Context const &context, float32_t time, float32_t interpolant);

		uint32_t hit(int32_t x, int32_t y) const;

	private:
		enum { BATCH_MAXIMUM = 250 };

		struct CopyParameters
		{
			gal::Parameter const *theSource = nullptr;
		};

		struct BlurParameters
		{
			gal::Parameter const *texelSize = nullptr;
			gal::Parameter const *theSource = nullptr;
		};

		struct PostParameters
		{
			gal::Parameter const *colorTarget = nullptr;
			gal::Parameter const *glowTarget = nullptr;
		};

		struct StarInstance
		{
			uint32_t id;
			gal::Vector4 parameters;
		};

		struct MeshInstance
		{
			uint32_t                         id;
			::lucid::gal::Vector3      position;
			float32_t                     scale;
			::lucid::gal::Quaternion   rotation;
			::lucid::gal::Color           color;
			::lucid::gal::Vector4    parameters;
		};

		int32_t _width = 0;
		int32_t _height = 0;

		float32_t _time = 0.f;
		float32_t _interpolant = 0.f;

		gal::Vector3 _viewPosition;
		gal::Matrix4x4 _viewProjMatrix;

		size_t _starCount = 0;
		std::unique_ptr<gal::VertexBuffer> _starInstances;
		std::unique_ptr<gigl::Mesh> _starMesh;

		std::shared_ptr<gigl::Mesh> _orbitMesh;

		gigl::Batched _batched;

		std::unique_ptr<gal::RenderTarget2D> _colorTarget;
		std::unique_ptr<gal::RenderTarget2D> _glowTarget;
		std::unique_ptr<gal::RenderTarget2D> _blurTarget[2];

		std::unique_ptr<gal::RenderTarget2D> _selectTarget;
		std::unique_ptr<gal::TargetReader2D> _selectReader;

		std::unique_ptr<gigl::Mesh> _clear;
		
		CopyParameters _copyParameters;
		std::unique_ptr<gigl::Mesh> _copy;
		
		BlurParameters _blurParameters;
		std::unique_ptr<gigl::Mesh> _blur;

		PostParameters _postParameters;
		std::unique_ptr<gigl::Mesh> _post;

		void batch(Frame *frame);

		bool cull(Frame *frame);

		void render(gigl::Context const &context);

		void copy(gal::RenderTarget2D *dst, gal::RenderTarget2D *src, gigl::Context const &context);

		void blur(gigl::Context const &context);

		void post(gigl::Context const &context);

		void resize();

		LUCID_PREVENT_COPY(Renderer);
		LUCID_PREVENT_ASSIGNMENT(Renderer);
	};

}	///	orbit
}	///	lucid
