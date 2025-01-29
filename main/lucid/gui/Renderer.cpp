#include "Renderer.h"
#include "Frame.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/gal/Pipeline.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

LUCID_GAL::VertexElement const format[] = {
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::  TYPE_VERTEX, 0, 0,  0, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::TYPE_INSTANCE, 1, 1,  0, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::TYPE_INSTANCE, 2, 1,  8, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::TYPE_INSTANCE, 3, 1, 16, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT2, LUCID_GAL::VertexElement::TYPE_INSTANCE, 4, 1, 24, },
	{ LUCID_GAL::VertexElement::FORMAT_FLOAT4, LUCID_GAL::VertexElement::TYPE_INSTANCE, 5, 1, 32, },
};

LUCID_GAL::Vector2 const vertices[] = {
	{ 0.f, 0.f, },
	{ 0.f, 1.f, },
	{ 1.f, 0.f, },
	{ 1.f, 1.f, },
};

uint16_t const indices[] = { 0, 1, 2, 3, };

LUCID_ANONYMOUS_END

LUCID_GUI_BEGIN

void Renderer::initialize()
{
	shutdown();

	_format = LUCID::gal::VertexFormat::create(::format, 6);

	_vertices = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_STATIC, 4, sizeof(LUCID_GAL::Vector2));
	_indices = LUCID_GAL::IndexBuffer::create(LUCID_GAL::IndexBuffer::USAGE_STATIC, LUCID_GAL::IndexBuffer::FORMAT_UINT16, 4);
	_instances = LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, BATCH_MAXIMUM, sizeof(Icon));
	
	std::memcpy(_vertices->lock(), ::vertices, 4 * sizeof(LUCID_GAL::Vector2));
	_vertices->unlock();

	std::memcpy(_indices->lock(), ::indices, 4 * sizeof(uint16_t));
	_indices->unlock();
}

void Renderer::shutdown()
{
	delete _format;
	_format = nullptr;

	delete _vertices;
	_vertices = nullptr;

	delete _indices;
	_indices = nullptr;

	delete _instances;
	_instances = nullptr;
}

void Renderer::render(Frame const *frame)
{
	_icons.clear();

	capusa(frame);

	// TBD: must set vertex and pixel shaders here

	size_t pos = 0;
	size_t count = std::min(size_t(BATCH_MAXIMUM), _icons.size() - pos);
	while (0 < count)
	{
		Icon *buffer = _instances->lock_as<Icon>();
		std::memcpy(buffer, &(_icons[pos]), count * sizeof(Icon));
		_instances->unlock();

#if false
		LUCID_GAL_PIPELINE.beginGeometry(_format);
			LUCID_GAL_PIPELINE.setVertexStream(0, _vertices);
			LUCID_GAL_PIPELINE.setVertexStream(1, _instances);
			LUCID_GAL_PIPELINE.setIndexStream(_indices);
			LUCID_GAL_PIPELINE.drawInstanced(LUCID_GAL::Pipeline::TOPOLOGY_TRIANGLE_STRIP, 4, 4, int(count));
		LUCID_GAL_PIPELINE.endGeometry(_format);
#endif

		pos = pos + count;
		count = std::min(size_t(BATCH_MAXIMUM), _icons.size() - pos);
	}
}

void Renderer::capusa(Frame const *frame)
{
	if (nullptr == frame)
		return;

	frame->accept(this);

	capusa(frame->nextSibling());
	capusa(frame->firstChild());
}

LUCID_GUI_END
