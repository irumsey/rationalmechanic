#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal.private/gal.d3d11/Buffer.h>

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
namespace d3d11 {

	///
	///
	///
	class IndexBuffer : public ::lucid::gal::IndexBuffer
	{
	public:
		IndexBuffer(USAGE usage, int32_t count);

		IndexBuffer(::lucid::core::Reader &reader);

		virtual ~IndexBuffer();

		virtual USAGE usage() const override;

		virtual int32_t count() const override;

		virtual uint16_t *lock(int32_t start = 0, int32_t count = 0) override;

		virtual void unlock() override;

		ID3D11Buffer *d3dBuffer() const;

	private:
		USAGE _usage = USAGE_UNDEFINED;
		Buffer<uint16_t> *_d3dBuffer = nullptr;

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

	inline int32_t IndexBuffer::count() const
	{
		return _d3dBuffer->count();
	}

	inline ID3D11Buffer *IndexBuffer::d3dBuffer() const
	{
		return _d3dBuffer->d3dBuffer();
	}

}	///	d3d11
}	///	gal
}	///	lucid