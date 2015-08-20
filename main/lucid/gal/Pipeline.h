#pragma once

#include <cstdint>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>

///
///
///
namespace lucid {
namespace gal {

	///
	///
	///

	class Statistics;
	class RenderTarget2D;
	class DepthTarget2D;
	class Program;
	class VertexFormat;
	class VertexBuffer;
	class IndexBuffer;

	///	Pipeline
	///
	///
	class Pipeline
	{
	public:
		enum TOPOLOGY
		{
			TOPOLOGY_POINTLIST,
			TOPOLOGY_LINELIST,
			TOPOLOGY_LINESTRIP,
			TOPOLOGY_TRIANGLELIST,
			TOPOLOGY_TRIANGLESTRIP,
		};

		virtual ~Pipeline() = 0 {}

		virtual Statistics &statistics() = 0;

		virtual Statistics const &statistics() const = 0;

		virtual void beginScene() = 0;

		virtual void endScene() = 0;

		virtual void beginProgram(Program const *program) = 0;

		virtual void endProgram(Program const *program) = 0;

		virtual void beginGeometry(VertexFormat const *format) = 0;

		virtual void endGeometry(VertexFormat const *format) = 0;

		virtual void setRenderTarget(int32_t index, RenderTarget2D *renderTarget) = 0;

		virtual void setDepthTarget(DepthTarget2D *depthtarget) = 0;

		virtual void restoreBackBuffer(bool color = true, bool depth = true) = 0;

		virtual void viewport(Viewport const &viewport) = 0;

		virtual Viewport const &viewport() const = 0;

		virtual void clear(bool clearTarget = true, bool clearDepth = true, bool clearStencil = true, Color const &color = Color(), float32_t depth = 1.f, uint8_t stencil = 0x00) = 0;

		virtual void setVertexStream(int32_t index, VertexBuffer const *buffer, int32_t start = 0) = 0;

		virtual void setIndexStream(IndexBuffer const *buffer) = 0;

		virtual void draw(TOPOLOGY topology, int32_t vertexCount, int32_t indexCount, int32_t indexStart = 0, int32_t indexOffset = 0) = 0;

		virtual void drawInstanced(TOPOLOGY topology, int vertexCount, int indexCount, int instanceCount, int indexStart = 0, int instanceStart = 0, int indexOffset = 0) = 0;

		static Pipeline &instance();

	protected:
		Pipeline() {}

		LUCID_PREVENT_COPY(Pipeline);
		LUCID_PREVENT_ASSIGNMENT(Pipeline);
	};

}	///	gal
}	///	lucid
