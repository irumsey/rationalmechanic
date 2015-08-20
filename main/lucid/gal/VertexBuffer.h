#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace gal {

	///	VertexBuffer
	///
	///	collection of vertices used to render geometry.
	class VertexBuffer
	{
	public:
		enum USAGE
		{
			USAGE_UNDEFINED = -1,

			USAGE_STATIC,
			USAGE_DYNAMIC
		};

		virtual ~VertexBuffer() = 0 {}

		virtual USAGE usage() const = 0;

		virtual int32_t count() const = 0;

		virtual int32_t stride() const = 0;

		virtual void *lock(int32_t start = 0, int32_t count = 0) = 0;

		virtual void unlock() = 0;

		static VertexBuffer *create(USAGE usage, int32_t count, int32_t stride);

		static VertexBuffer *create(::lucid::core::Reader &reader);

	protected:
		VertexBuffer() {}

		LUCID_PREVENT_COPY(VertexBuffer);
		LUCID_PREVENT_ASSIGNMENT(VertexBuffer);
	};

}	///	gal
}	///	lucid
