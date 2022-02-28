#include "VertexBuffer.h"
#include <lucid/gal/VertexBuffer.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}	///	anonymous

namespace Lucid {
namespace GAL {

	VertexBuffer::VertexBuffer(VertexUsage usage, int count, int stride)
	{
		try
		{
			_internal = gal::VertexBuffer::create(static_cast<gal::VertexBuffer::USAGE>(usage), count, stride);
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	VertexBuffer::~VertexBuffer()
	{
		this->!VertexBuffer();
	}

	VertexBuffer::!VertexBuffer()
	{
		delete _internal;
	}

	VertexUsage VertexBuffer::usage()
	{
		return static_cast<VertexUsage>(_internal->usage());
	}

	int VertexBuffer::count()
	{
		return _internal->count();
	}

	int VertexBuffer::stride()
	{
		return _internal->stride();
	}

	void *VertexBuffer::lock()
	{
		return _internal->lock();
	}

	void VertexBuffer::unlock()
	{
		_internal->unlock();
	}

} /// GAL
} /// Lucid
