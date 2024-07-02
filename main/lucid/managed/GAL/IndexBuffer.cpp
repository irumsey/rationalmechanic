#include "IndexBuffer.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace Lucid {
namespace GAL {

	IndexBuffer::IndexBuffer(IndexUsage usage, IndexFormat format, int count)
	{
		try
		{
			_internal = LUCID_GAL::IndexBuffer::create(static_cast<LUCID_GAL::IndexBuffer::USAGE>(usage), static_cast<LUCID_GAL::IndexBuffer::FORMAT>(format), count);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	IndexBuffer::~IndexBuffer()
	{
		this->!IndexBuffer();
	}

	IndexBuffer::!IndexBuffer()
	{
		delete _internal;
	}

	IndexUsage IndexBuffer::usage()
	{
		return static_cast<IndexUsage>(_internal->usage());
	}

	IndexFormat IndexBuffer::format()
	{
		return static_cast<IndexFormat>(_internal->format());
	}

	int IndexBuffer::count()
	{
		return _internal->count();
	}

	void *IndexBuffer::lock()
	{
		return _internal->lock();
	}

	void IndexBuffer::unlock()
	{
		_internal->unlock();
	}

} /// GAL
} /// Lucid
