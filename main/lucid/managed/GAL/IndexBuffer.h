#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class IndexBuffer;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	IndexUsage
	///
	///	Note: must match unmanaged
	public enum class IndexUsage
	{
		UNDEFINED = -1,
		STATIC = 0,
		DYNAMIC = 1,
	};

	///	IndexFormat
	///
	///	Note: must match unmanaged
	public enum class IndexFormat
	{
		UNDEFINED = -1,
		UINT16 = 0,
		UINT32 = 1,
	};

	///	VertexBuffer
	///
	///
	public ref class IndexBuffer
	{
	public:
		IndexBuffer(IndexUsage usage, IndexFormat format, int count);

		~IndexBuffer();

		!IndexBuffer();

		IndexUsage usage();

		IndexFormat format();

		int count();

		void *lock();

		void unlock();

		property LUCID_GAL::IndexBuffer &ref { LUCID_GAL::IndexBuffer &get() { return *ptr; } }

		property LUCID_GAL::IndexBuffer *ptr { LUCID_GAL::IndexBuffer *get() { return _internal; } }

	private:
		LUCID_GAL::IndexBuffer *_internal = nullptr;

	};

} /// GAL
} /// Lucid


