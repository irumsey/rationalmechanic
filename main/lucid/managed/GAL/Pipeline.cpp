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
#include <lucid/gal/Defines.h>

namespace Lucid {
namespace GAL {

	void Pipeline::initialize(int32_t width, int32_t height, int32_t samples, void *window)
	{
		LUCID_GAL::System &system = LUCID_GAL::System::instance();

		system.initialize(true, width, height, samples, window);
	}

	void Pipeline::shutdown()
	{
		LUCID_GAL::System &system = LUCID_GAL::System::instance();

		system.shutdown();
	}

	void Pipeline::resize(int32_t width, int32_t height)
	{
		LUCID_GAL::System &system = LUCID_GAL::System::instance();

		system.resize(width, height);
	}

	void  Pipeline::beginScene()
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.beginScene();
	}

	void Pipeline::endScene()
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.endScene();
	}

	void Pipeline::setRenderTarget(int index, RenderTarget2D ^renderTarget)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.setRenderTarget(index, renderTarget->ptr);
	}

	void Pipeline::setUnorderedTarget(Unordered2D ^unordered)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.setUnorderedTarget(unordered->ptr);
	}

	void Pipeline::setDepthTarget(DepthTarget2D ^depthTarget)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.setDepthTarget(depthTarget->ptr);
	}

	void Pipeline::restoreBackbuffer(bool color, bool depth, bool unordered)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.restoreBackBuffer(color, depth, unordered);
	}

	Viewport ^Pipeline::getViewport()
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		return gcnew Viewport(pipeline.viewport());
	}

	void Pipeline::setViewport(Viewport ^viewport)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.viewport(viewport->ref);
	}

	void Pipeline::clear(Color ^color, float32_t depth)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.clear(true, true, true, LUCID_GAL::Color(color->r, color->g, color->b, color->a), depth, 0x00);
	}

	void Pipeline::clear(Color ^color)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.clear(true, false, false, LUCID_GAL::Color(color->r, color->g, color->b, color->a));
	}

	void Pipeline::clear(float32_t depth)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.clear(false, true, true, LUCID_GAL::Color(1, 0, 1, 1), depth, 0x00);
	}

	void Pipeline::beginProgram(Program ^program)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.beginProgram(program->ptr);
	}

	void Pipeline::endProgram(Program ^program)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.endProgram(program->ptr);
	}

	void Pipeline::beginGeometry(VertexFormat ^format)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.beginGeometry(format->ptr);
	}

	void Pipeline::endGeometry(VertexFormat ^format)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.endGeometry(format->ptr);
	}

	void Pipeline::setVertexStream(int index, VertexBuffer ^buffer)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.setVertexStream(index, buffer->ptr);
	}

	void Pipeline::setIndexStream(IndexBuffer ^buffer)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.setIndexStream(buffer->ptr);
	}

	void Pipeline::draw(Topology topology, int vertexCount, int indexCount)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.draw(static_cast<LUCID_GAL::Pipeline::TOPOLOGY>(topology), vertexCount, indexCount);
	}

	void Pipeline::drawInstanced(Topology topology, int vertexCount, int indexCount, int instanceCount)
	{
		LUCID_GAL::Pipeline &pipeline = LUCID_GAL::Pipeline::instance();

		pipeline.drawInstanced(static_cast<LUCID_GAL::Pipeline::TOPOLOGY>(topology), vertexCount, indexCount, instanceCount);
	}

} /// GAL
} /// Lucid
