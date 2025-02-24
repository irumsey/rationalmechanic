#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

LUCID_GAL_BEGIN

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

	enum FORMAT
	{
		FORMAT_UNDEFINED = -1,

		FORMAT_UINT16,
		FORMAT_UINT32,
	};

	virtual ~IndexBuffer() = 0 {}

	virtual USAGE usage() const = 0;

	virtual FORMAT format() const = 0;

	virtual int32_t count() const = 0;

	template<typename T> T *lock_as(int32_t start = 0, int32_t count = 0);

	virtual void *lock(int32_t start = 0, int32_t count = 0) = 0;

	virtual void unlock() = 0;

	static IndexBuffer *create(USAGE usage, FORMAT format, int32_t count);

	static IndexBuffer *create(LUCID_CORE::Unserializer &reader);

protected:
	IndexBuffer() {}

	LUCID_PREVENT_COPY(IndexBuffer);
	LUCID_PREVENT_ASSIGNMENT(IndexBuffer);
};

template<typename T> inline T *IndexBuffer::lock_as(int32_t start, int32_t count)
{
	return reinterpret_cast<T*>(lock(start, count));
}

LUCID_GAL_END
