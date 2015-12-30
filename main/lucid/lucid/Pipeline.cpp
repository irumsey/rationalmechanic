#include "Pipeline.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "Program.h"
#include "DepthTarget2D.h"
#include "RenderTarget2D.h"
#include "Unordered2D.h"
#include "Types.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}	///	anonymous

namespace lucid {

	void Pipeline::initialize(int32_t width, int32_t height, int32_t samples, void *window)
	{
		gal::System &system = gal::System::instance();

		system.initialize(true, width, height, samples, window);
	}

	void Pipeline::shutdown()
	{
		gal::System &system = gal::System::instance();

		system.shutdown();
	}

	void Pipeline::resize(int32_t width, int32_t height)
	{
		gal::System &system = gal::System::instance();

		system.resize(width, height);
	}

	void  Pipeline::beginScene()
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginScene();
	}

	void Pipeline::endScene()
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.endScene();
	}

	void Pipeline::setRenderTarget(int index, RenderTarget2D ^renderTarget)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.setRenderTarget(index, renderTarget->ptr);
	}

	void Pipeline::setUnorderedTarget(Unordered2D ^unordered)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.setUnorderedTarget(unordered->ptr);
	}

	void Pipeline::setDepthTarget(DepthTarget2D ^depthTarget)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.setDepthTarget(depthTarget->ptr);
	}

	void Pipeline::restoreBackbuffer(bool color, bool depth, bool unordered)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.restoreBackBuffer(color, depth, unordered);
	}

	Viewport ^Pipeline::getViewport()
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		return gcnew Viewport(pipeline.viewport());
	}

	void Pipeline::setViewport(Viewport ^viewport)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.viewport(viewport->ref);
	}

	void Pipeline::clear(Color ^color, float32_t depth)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.clear(true, true, true, gal::Color(color->r, color->g, color->b, color->a), depth, 0x00);
	}

	void Pipeline::clear(Color ^color)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.clear(true, false, false, gal::Color(color->r, color->g, color->b, color->a));
	}

	void Pipeline::clear(float32_t depth)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.clear(false, true, true, gal::Color(1, 0, 1, 1), depth, 0x00);
	}

	void Pipeline::beginProgram(Program ^program)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginProgram(program->ptr);
	}

	void Pipeline::endProgram(Program ^program)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.endProgram(program->ptr);
	}

	void Pipeline::beginGeometry(VertexFormat ^format)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.beginGeometry(format->ptr);
	}

	void Pipeline::endGeometry(VertexFormat ^format)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.endGeometry(format->ptr);
	}

	void Pipeline::setVertexStream(int index, VertexBuffer ^buffer)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.setVertexStream(index, buffer->ptr);
	}

	void Pipeline::setIndexStream(IndexBuffer ^buffer)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.setIndexStream(buffer->ptr);
	}

	void Pipeline::draw(Topology topology, int vertexCount, int indexCount)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.draw(static_cast<gal::Pipeline::TOPOLOGY>(topology), vertexCount, indexCount);
	}

	void Pipeline::drawInstanced(Topology topology, int vertexCount, int indexCount, int instanceCount)
	{
		gal::Pipeline &pipeline = gal::Pipeline::instance();

		pipeline.drawInstanced(static_cast<gal::Pipeline::TOPOLOGY>(topology), vertexCount, indexCount, instanceCount);
	}

}	///  lucid
