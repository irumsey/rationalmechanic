#include "VertexBuffer.h"
#include <lucid/gal/VertexBuffer.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace Lucid {
namespace GAL {

	VertexBuffer::VertexBuffer(VertexUsage usage, int count, int stride)
	{
		try
		{
			_internal = LUCID_GAL::VertexBuffer::create(static_cast<LUCID_GAL::VertexBuffer::USAGE>(usage), count, stride);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
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
