#include "VertexBuffer.h"
#include "Pipeline.h"
#include <lucid/core/Unserializer.h>

LUCID_GAL_BEGIN

VertexBuffer *VertexBuffer::create(USAGE usage, int32_t count, int32_t stride)
{
	return new LUCID_GAL_D3D11::VertexBuffer(usage, count, stride);
}

VertexBuffer *VertexBuffer::create(LUCID_CORE::Unserializer &reader)
{
	return new LUCID_GAL_D3D11::VertexBuffer(reader);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

VertexBuffer::VertexBuffer(USAGE usage, int32_t count, int32_t stride)
	: _usage(usage)
{
	initialize(count, stride);
}

VertexBuffer::VertexBuffer(LUCID_CORE::Unserializer &reader)
{
	_usage = reader.read<USAGE>();

	int32_t stride = reader.read<int32_t>();
	int32_t size = reader.read<int32_t>();
	
	LUCID_VALIDATE(0 == (size % stride), "");
	int32_t count = size / stride;

	initialize(count, stride);

	reader.read(_d3dBuffer->lock(), size);
	_d3dBuffer->unlock();
}

VertexBuffer::~VertexBuffer()
{
	shutdown();
}

void *VertexBuffer::lock(int32_t start, int32_t count)
{
	return _d3dBuffer->lock(start, count);
}

void VertexBuffer::unlock()
{
	_d3dBuffer->unlock();
}

void VertexBuffer::initialize(int32_t count, int32_t stride)
{
	switch (_usage)
	{
	case USAGE_STATIC:
		_d3dBuffer = new StaticBuffer<D3D11_BIND_VERTEX_BUFFER>(count, stride);
		break;
	case USAGE_DYNAMIC:
		_d3dBuffer = new DynamicBuffer<D3D11_BIND_VERTEX_BUFFER>(count, stride);
		break;
	default:
		LUCID_THROW("invalid vertex buffer usage specified");
		break;
	}

	++galConcreteStatistic(vertexBuffers);

	_size = _d3dBuffer->count() * _d3dBuffer->stride();
	galConcreteStatistic(vertexMemory) += _size;
}

void VertexBuffer::shutdown()
{
	galConcreteStatistic(vertexMemory) -= _size;
	--galConcreteStatistic(vertexBuffers);

	delete _d3dBuffer;
	_d3dBuffer = nullptr;
}

LUCID_GAL_D3D11_END
