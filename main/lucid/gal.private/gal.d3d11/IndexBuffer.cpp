#include "IndexBuffer.h"
#include "Pipeline.h"
#include <lucid/core/Reader.h>

///
///
///
namespace lucid {
namespace gal {

	IndexBuffer *IndexBuffer::create(USAGE usage, int32_t count)
	{
		return new ::lucid::gal::d3d11::IndexBuffer(usage, count);
	}

	IndexBuffer *IndexBuffer::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::IndexBuffer(reader);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	IndexBuffer::IndexBuffer(USAGE usage, int32_t count)
		: _usage(usage)
	{
		initialize(count);
	}

	IndexBuffer::IndexBuffer(::lucid::core::Reader &reader)
	{
		reader.read(&_usage, sizeof(USAGE));

		int32_t count = reader.read<int32_t>();
		initialize(count);

		reader.read(_d3dBuffer->lock(), count * sizeof(uint16_t));
		_d3dBuffer->unlock();
	}

	IndexBuffer::~IndexBuffer()
	{
		shutdown();
	}

	uint16_t *IndexBuffer::lock(int32_t start, int32_t count)
	{
		return _d3dBuffer->lock(start, count);
	}

	void IndexBuffer::unlock()
	{
		_d3dBuffer->unlock();
	}

	void IndexBuffer::initialize(int32_t count)
	{
		switch (_usage)
		{
		case USAGE_STATIC:
			_d3dBuffer = new StaticBuffer<uint16_t, D3D11_BIND_INDEX_BUFFER>(count, sizeof(uint16_t));
			break;
		case USAGE_DYNAMIC:
			_d3dBuffer = new DynamicBuffer<uint16_t, D3D11_BIND_INDEX_BUFFER>(count, sizeof(uint16_t));
			break;
		default:
			LUCID_THROW("invalid index buffer type specified");
			break;
		}

		++galConcreteStatistic(indexBuffers);

		_size = 2 * _d3dBuffer->count();
		galConcreteStatistic(indexMemory) += _size;
	}

	void IndexBuffer::shutdown()
	{
		galConcreteStatistic(indexMemory) -= _size;
		--galConcreteStatistic(indexBuffers);

		delete _d3dBuffer;
		_d3dBuffer = nullptr;
	}

}	///	d3d11
}	///	gal
}	///	lucid