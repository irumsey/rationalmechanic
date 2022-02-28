#pragma once

namespace lucid {
namespace gal {

	class IndexBuffer;

}	///	gal
}	///	lucid

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

		property ::lucid::gal::IndexBuffer &ref { ::lucid::gal::IndexBuffer &get() { return *ptr; } }

		property ::lucid::gal::IndexBuffer *ptr { ::lucid::gal::IndexBuffer *get() { return _internal; } }

	private:
		::lucid::gal::IndexBuffer *_internal = nullptr;

	};

} /// GAL
} /// Lucid


