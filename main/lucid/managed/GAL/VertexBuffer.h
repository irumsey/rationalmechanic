#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class VertexBuffer;

LUCID_GAL_END

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

		property LUCID_GAL::VertexBuffer &ref { LUCID_GAL::VertexBuffer &get() { return *ptr; } }

		property LUCID_GAL::VertexBuffer *ptr { LUCID_GAL::VertexBuffer *get() { return _internal; } }

	private:
		LUCID_GAL::VertexBuffer *_internal = nullptr;

	};

} /// GAL
} /// Lucid
