#pragma once

namespace lucid {
namespace gal {

	class VertexBuffer;

}	///	gal
}	///	lucid

namespace Lucid {
namespace GAL {

	///	VertexUsage
	///
	///
	public enum class VertexUsage
	{
		UNDEFINED = -1,
		STATIC = 0,
		DYNAMIC = 1,
	};

	///	VertexBuffer
	///
	///
	public ref class VertexBuffer
	{
	public:
		VertexBuffer(VertexUsage usage, int count, int stride);

		~VertexBuffer();

		!VertexBuffer();

		VertexUsage usage();

		int count();

		int stride();

		void *lock();

		void unlock();

		property ::lucid::gal::VertexBuffer &ref { ::lucid::gal::VertexBuffer &get() { return *ptr; } }

		property ::lucid::gal::VertexBuffer *ptr { ::lucid::gal::VertexBuffer *get() { return _internal; } }

	private:
		::lucid::gal::VertexBuffer *_internal = nullptr;

	};

} /// GAL
} /// Lucid
