#pragma once

#include <string>
#include <memory>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Profiler.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Batched.h>
#include <lucid/orbit/Types.h>
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

	class Mesh;

}	///	gigl
}	/// lucid

namespace lucid {
namespace orbit {

	///
	///
	///
	class Renderer : public Algorithm
	{
	public:
		///	SELECT
		/// 
		///	Codes used to render into the render target.
		/// Used for "pixel perfect" selection.
		enum SELECT
		{
			SELECT_STAR		= 0x01,
			SELECT_FRAME	= 0x02,
			SELECT_ORBIT	= 0x03,
			SELECT_CAMERA	= 0x04,
			SELECT_OTHER	= 0x0f,
		};
		enum { SELECT_SHIFT = 28 };
		enum { SELECT_MASK  = 0x0fffffff};

		Renderer();

		virtual ~Renderer();

		virtual void evaluate(DynamicPoint *point) override;

		virtual void evaluate(OrbitalBody *body) override;

		virtual void evaluate(DynamicBody *body) override;

		virtual void evaluate(CameraFrame *camera) override;

		void initialize(std::string const &path);

		void shutdown();

		void render(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t time, scalar_t interpolant);

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
			uint32_t                id;
			gal::Vector3      position;
			float32_t            scale;
			gal::Quaternion   rotation;
			gal::Color           color;
			gal::Vector4    parameters;
		};

		struct CalloutInstance
		{
			gal::Vector2  position;
			gal::Vector4 dimension;
			gal::Color       color;
		};

		int32_t _width = 0;
		int32_t _height = 0;

		float32_t _time = 0.f;
		float32_t _interpolant = 0.f;
		gal::Vector3 _cameraPosition;

		gigl::Context _renderContext;

		size_t _starCount = 0;
		std::unique_ptr<gal::VertexBuffer> _starInstances;
		std::unique_ptr<gigl::Mesh> _starMesh;

		std::shared_ptr<gigl::Mesh> _orbitMesh;
		std::shared_ptr<gigl::Mesh> _calloutMesh;

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

		void render();

		void copy(gal::RenderTarget2D *dst, gal::RenderTarget2D *src);

		void blur();

		void post();

		void resize();

		LUCID_PREVENT_COPY(Renderer);
		LUCID_PREVENT_ASSIGNMENT(Renderer);
	};

}	///	orbit
}	///	lucid
