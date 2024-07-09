#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>
#include <lucid/gal.private/gal.d3d11/Buffer.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class VertexBuffer : public LUCID_GAL::VertexBuffer
{
public:
	VertexBuffer(USAGE usage, int32_t count, int32_t stride);

	VertexBuffer(LUCID_CORE::Reader &reader);

	virtual ~VertexBuffer();

	virtual USAGE usage() const override;

	virtual int32_t count() const override;

	virtual int32_t stride() const override;

	virtual void *lock(int32_t start = 0, int32_t count = 0) override;

	virtual void unlock() override;

	ID3D11Buffer *d3dBuffer() const;

private:
	USAGE _usage = USAGE_UNDEFINED;
	Buffer *_d3dBuffer = nullptr;

	int32_t _size = 0;

	void initialize(int32_t count, int32_t stride);

	void shutdown();

	LUCID_PREVENT_COPY(VertexBuffer);
	LUCID_PREVENT_ASSIGNMENT(VertexBuffer);
};

inline VertexBuffer::USAGE VertexBuffer::usage() const
{
	return _usage;
}

inline int32_t VertexBuffer::count() const
{
	return _d3dBuffer->count();
}

inline int32_t VertexBuffer::stride() const
{
	return _d3dBuffer->stride();
}

inline ID3D11Buffer *VertexBuffer::d3dBuffer() const
{
	return _d3dBuffer->d3dBuffer();
}

LUCID_GAL_D3D11_END