#pragma once

#include <lucid/core/Types.h>

namespace Lucid {
namespace GAL {

	ref class Color;
	ref class Viewport;
	ref class Unordered2D;
	ref class RenderTarget2D;
	ref class DepthTarget2D;
	ref class Program;
	ref class VertexFormat;
	ref class VertexBuffer;
	ref class IndexBuffer;

	public enum class Topology
	{
		POINT_LIST = 0,
		LINE_LIST = 1,
		LINE_STRIP = 2,
		TRIANGLE_LIST = 3,
		TRIANGLE_STRIP = 4,
		TRIANGLE_ADJACENCY = 5,
	};

	public ref class Pipeline
	{
	public:
		virtual ~Pipeline() {}

		static void initialize(int32_t width, int32_t height, int32_t samples, void *window);

		static void shutdown();

		static void resize(int32_t width, int32_t height);

		static void beginScene();

		static void endScene();

		static void setRenderTarget(int index, RenderTarget2D ^renderTarget);

		static void setUnorderedTarget(Unordered2D ^unordered);

		static void setDepthTarget(DepthTarget2D ^depthTarget);

		static void restoreBackbuffer(bool color, bool depth, bool unordered);

		static Viewport ^getViewport();

		static void setViewport(Viewport ^viewport);

		static void clear(Color ^color, float32_t depth);

		static void clear(Color ^color);

		static void clear(float32_t depth);

		static void beginProgram(Program ^program);

		static void endProgram(Program ^program);

		static void beginGeometry(VertexFormat ^format);

		static void endGeometry(VertexFormat ^format);

		static void setVertexStream(int index, VertexBuffer ^buffer);

		static void setIndexStream(IndexBuffer ^buffer);

		static void draw(Topology topology, int vertexCount, int indexCount);

		static void drawInstanced(Topology topology, int vertexCount, int indexCount, int instanceCount);

	protected:
		Pipeline() {}

	};

} /// GAL
} /// Lucid
