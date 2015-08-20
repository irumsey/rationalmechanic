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

	///	IndexBuffer
	///
	///	collection of indices used to render geometry.
	class IndexBuffer
	{
	public:
		enum USAGE
		{
			USAGE_UNDEFINED = -1,

			USAGE_STATIC,
			USAGE_DYNAMIC,
		};

		virtual ~IndexBuffer() = 0 {}

		virtual USAGE usage() const = 0;

		virtual int32_t count() const = 0;

		virtual uint16_t *lock(int32_t start = 0, int32_t count = 0) = 0;

		virtual void unlock() = 0;

		static IndexBuffer *create(USAGE usage, int32_t size);

		static IndexBuffer *create(::lucid::core::Reader &reader);

	protected:
		IndexBuffer() {}

		LUCID_PREVENT_COPY(IndexBuffer);
		LUCID_PREVENT_ASSIGNMENT(IndexBuffer);
	};

}	///	gal
}	///	lucid
