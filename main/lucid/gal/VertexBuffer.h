#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

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

	template<typename T> T *lock_as(int32_t start = 0, int32_t count = 0);

	virtual void *lock(int32_t start = 0, int32_t count = 0) = 0;

	virtual void unlock() = 0;

	static VertexBuffer *create(USAGE usage, int32_t count, int32_t stride);

	static VertexBuffer *create(LUCID_CORE::Reader &reader);

protected:
	VertexBuffer() {}

	LUCID_PREVENT_COPY(VertexBuffer);
	LUCID_PREVENT_ASSIGNMENT(VertexBuffer);
};

template<typename T> inline T *VertexBuffer::lock_as(int32_t start, int32_t count)
{
	return reinterpret_cast<T*>(lock(start, count));
}

LUCID_GAL_END