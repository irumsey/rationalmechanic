#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal.private/gal.d3d11/Utility.h>
#include <lucid/gal.private/gal.d3d11/System.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///
	template<class T> class Buffer
	{
	public:
		virtual ~Buffer() {}

		virtual int32_t count() const = 0;

		virtual int32_t stride() const = 0;

		virtual T* lock(int32_t start = 0, int32_t count = 0) = 0;

		virtual void unlock() = 0;

		virtual ID3D11Buffer *d3dBuffer() const = 0;

	protected:
		Buffer() {}

		LUCID_PREVENT_COPY(Buffer);
		LUCID_PREVENT_ASSIGNMENT(Buffer);
	};

	///
	///
	///
	template<class TYPE, D3D11_BIND_FLAG BINDING> class StaticBuffer : public Buffer <TYPE>
	{
	public:
		StaticBuffer(int32_t count, int32_t stride)
			: _count(count)
			, _stride(stride)
		{
			::memset(&_d3dDesc, 0, sizeof(D3D11_BUFFER_DESC));

			_d3dDesc.Usage = D3D11_USAGE_DEFAULT;
			_d3dDesc.ByteWidth = _count * _stride;
			_d3dDesc.BindFlags = BINDING;

			_buffer = new uint8_t[_count * _stride];
		}

		virtual ~StaticBuffer()
		{
			safeRelease(_d3dBuffer);
			delete[] _buffer;
		}

		virtual int32_t count() const override
		{
			return _count;
		}

		virtual int32_t stride() const override
		{
			return _stride;
		}

		virtual TYPE* lock(int32_t start = 0, int32_t count = 0) override
		{
			return reinterpret_cast<TYPE*>(_buffer + start * _stride);
		}

		virtual void unlock() override
		{
			safeRelease(_d3dBuffer);

			D3D11_SUBRESOURCE_DATA data;

			data.pSysMem = _buffer;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			HRESULT hResult = d3d11ConcreteDevice->CreateBuffer(&_d3dDesc, &data, &_d3dBuffer);
			GAL_VALIDATE_HRESULT(hResult, "unable to create buffer");
		}

		virtual ID3D11Buffer *d3dBuffer() const override
		{
			return _d3dBuffer;
		}

	private:
		int32_t _count = 0;
		int32_t _stride = 0;
		uint8_t *_buffer = 0;

		ID3D11Buffer *_d3dBuffer = 0;
		D3D11_BUFFER_DESC _d3dDesc;

		LUCID_PREVENT_COPY(StaticBuffer);
		LUCID_PREVENT_ASSIGNMENT(StaticBuffer);
	};

	///
	///
	///
	template<class TYPE, D3D11_BIND_FLAG BINDING> class DynamicBuffer : public Buffer <TYPE>
	{
	public:
		DynamicBuffer(int32_t count, int32_t stride)
			: _count(count)
			, _stride(stride)
		{
			D3D11_BUFFER_DESC d3dDesc;
			::memset(&d3dDesc, 0, sizeof(D3D11_BUFFER_DESC));

			d3dDesc.Usage = D3D11_USAGE_DYNAMIC;
			d3dDesc.ByteWidth = count * stride;
			d3dDesc.BindFlags = BINDING;
			d3dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			HRESULT hResult = d3d11ConcreteDevice->CreateBuffer(&d3dDesc, nullptr, &_d3dBuffer);
			GAL_VALIDATE_HRESULT(hResult, "unable to create buffer");
		}

		virtual ~DynamicBuffer()
		{
			safeRelease(_d3dBuffer);
		}

		virtual int32_t count() const override
		{
			return _count;
		}

		virtual int32_t stride() const override
		{
			return _stride;
		}

		virtual TYPE* lock(int32_t start = 0, int32_t count = 0) override
		{
			D3D11_MAPPED_SUBRESOURCE mapped;
			HRESULT hResult = d3d11ConcreteContext->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			GAL_VALIDATE_HRESULT(hResult, "unable to lock buffer");

			_locked = true;

			return reinterpret_cast<TYPE*>(reinterpret_cast<uint8_t*>(mapped.pData) + start * _stride);
		}

		virtual void unlock() override
		{
			d3d11ConcreteContext->Unmap(_d3dBuffer, 0);
			_locked = false;
		}

		virtual ID3D11Buffer *d3dBuffer() const override
		{
			return _d3dBuffer;
		}

	private:
		int32_t _count = 0;
		int32_t _stride = 0;

		bool _locked = false;

		ID3D11Buffer *_d3dBuffer = nullptr;

		LUCID_PREVENT_COPY(DynamicBuffer);
		LUCID_PREVENT_ASSIGNMENT(DynamicBuffer);
	};

}	///	d3d11
}	///	gal
}	///	lucid