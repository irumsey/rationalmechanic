#include "IndexBuffer.h"
#include <lucid/gal/IndexBuffer.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}	///	anonymous

namespace lucid {

	IndexBuffer::IndexBuffer(IndexUsage usage, IndexFormat format, int count)
	{
		try
		{
			_internal = gal::IndexBuffer::create(static_cast<gal::IndexBuffer::USAGE>(usage), static_cast<gal::IndexBuffer::FORMAT>(format), count);
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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

}	///  lucid
