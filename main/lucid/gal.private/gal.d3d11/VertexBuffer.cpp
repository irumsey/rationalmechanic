#include "VertexBuffer.h"
#include "Pipeline.h"

///
///
///
namespace lucid {
namespace gal {

	VertexBuffer *VertexBuffer::create(USAGE usage, int32_t count, int32_t stride)
	{
		return new ::lucid::gal::d3d11::VertexBuffer(usage, count, stride);
	}

	VertexBuffer *VertexBuffer::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::VertexBuffer(reader);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	VertexBuffer::VertexBuffer(USAGE usage, int32_t count, int32_t stride)
		: _usage(usage)
	{
		initialize(count, stride);
	}

	VertexBuffer::VertexBuffer(::lucid::core::Reader &reader)
	{
		reader.read(&_usage, sizeof(_usage));

		int32_t count = reader.read<int32_t>();
		int32_t stride = reader.read<int32_t>();

		initialize(count, stride);

		reader.read(_d3dBuffer->lock(), stride * count);
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

}	///	d3d11
}	///	gal
}	///	lucid
