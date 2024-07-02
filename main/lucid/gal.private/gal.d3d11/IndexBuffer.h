#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>
#include <lucid/gal.private/gal.d3d11/Buffer.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class IndexBuffer : public ::lucid::gal::IndexBuffer
{
public:
	IndexBuffer(USAGE usage, FORMAT format, int32_t count);

	IndexBuffer(::lucid::core::Reader &reader);

	virtual ~IndexBuffer();

	virtual FORMAT format() const override;

	virtual USAGE usage() const override;

	virtual int32_t count() const override;

	virtual void *lock(int32_t start = 0, int32_t count = 0) override;

	virtual void unlock() override;

	ID3D11Buffer *d3dBuffer() const;

private:
	USAGE _usage = USAGE_UNDEFINED;
	FORMAT _format = FORMAT_UNDEFINED;
	Buffer *_d3dBuffer = nullptr;

	int32_t _size = 0;

	void initialize(int32_t count);

	void shutdown();

	LUCID_PREVENT_COPY(IndexBuffer);
	LUCID_PREVENT_ASSIGNMENT(IndexBuffer);
};

inline IndexBuffer::USAGE IndexBuffer::usage() const
{
	return _usage;
}

inline IndexBuffer::FORMAT IndexBuffer::format() const
{
	return _format;
}

inline int32_t IndexBuffer::count() const
{
	return _d3dBuffer->count();
}

inline ID3D11Buffer *IndexBuffer::d3dBuffer() const
{
	return _d3dBuffer->d3dBuffer();
}

LUCID_GAL_D3D11_END