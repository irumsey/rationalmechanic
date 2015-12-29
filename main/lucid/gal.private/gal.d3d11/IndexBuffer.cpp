#include "IndexBuffer.h"
#include "Pipeline.h"
#include <lucid/core/Reader.h>

///
///
///
namespace /* anonymous */
{
	
	/// ENUM LOOKUP
	int32_t const INDEX_SIZE[] = { 2, 4, };

}

///
///
///
namespace lucid {
namespace gal {

	IndexBuffer *IndexBuffer::create(USAGE usage, FORMAT format, int32_t count)
	{
		return new ::lucid::gal::d3d11::IndexBuffer(usage, format, count);
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

	IndexBuffer::IndexBuffer(USAGE usage, FORMAT format, int32_t count)
		: _usage(usage)
		, _format(format)
	{
		initialize(count);
	}

	IndexBuffer::IndexBuffer(::lucid::core::Reader &reader)
	{
		reader.read(&_usage, sizeof(USAGE));
		reader.read(&_format, sizeof(FORMAT));

		int32_t count = reader.read<int32_t>();
		initialize(count);

		reader.read(_d3dBuffer->lock(), count * INDEX_SIZE[_format]);
		_d3dBuffer->unlock();
	}

	IndexBuffer::~IndexBuffer()
	{
		shutdown();
	}

	void *IndexBuffer::lock(int32_t start, int32_t count)
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
			_d3dBuffer = new StaticBuffer<D3D11_BIND_INDEX_BUFFER>(count, INDEX_SIZE[_format]);
			break;
		case USAGE_DYNAMIC:
			_d3dBuffer = new DynamicBuffer<D3D11_BIND_INDEX_BUFFER>(count, INDEX_SIZE[_format]);
			break;
		default:
			LUCID_THROW("invalid index buffer type specified");
			break;
		}

		++galConcreteStatistic(indexBuffers);

		_size = INDEX_SIZE[_format] * _d3dBuffer->count();
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