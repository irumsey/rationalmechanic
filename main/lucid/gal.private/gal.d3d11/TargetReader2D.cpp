#include "TargetReader2D.h"
#include "RenderTarget2D.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

TargetReader2D *TargetReader2D::create(LUCID_GAL::RenderTarget2D const *target, int32_t width, int32_t height)
{
	return new LUCID_GAL_D3D11::TargetReader2D(target, width, height);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

TargetReader2D::TargetReader2D(LUCID_GAL::RenderTarget2D const *target, int32_t width, int32_t height)
	: _width(width), _height(height)
{
	try
	{
		LUCID_GAL_D3D11::RenderTarget2D const *concrete = static_cast<LUCID_GAL_D3D11::RenderTarget2D const *>(target);
		_srcTexture = concrete->d3dTexture();

		D3D11_TEXTURE2D_DESC descTarget;
		_srcTexture->GetDesc(&descTarget);

		_srcWidth = descTarget.Width;
		_srcHeight = descTarget.Height;

		D3D11_TEXTURE2D_DESC descReader;
		::memset(&descReader, 0, sizeof(D3D11_TEXTURE2D_DESC));

		descReader.Usage = D3D11_USAGE_STAGING;
		descReader.Format = descTarget.Format;
		descReader.Width = width;
		descReader.Height = height;
		descReader.ArraySize = 1;
		descReader.SampleDesc = descTarget.SampleDesc;
		descReader.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descReader, nullptr, &_stagingTexture);
		GAL_VALIDATE_HRESULT(hResult, "unable to create render target reader");

		_data = new uint8_t[TEXEL_SIZE * _width * _height];
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

TargetReader2D::~TargetReader2D()
{
	shutdown();
}

uint8_t const *TargetReader2D::read() const
{
	D3D11_BOX box = { 0, 0, 0, UINT(_srcWidth), UINT(_srcHeight), 1, };
	d3d11ConcreteContext->CopySubresourceRegion(_stagingTexture, 0, 0, 0, 0, _srcTexture, 0, &box);

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hResult = d3d11ConcreteContext->Map(_stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
	GAL_VALIDATE_HRESULT(hResult, "unable to map staging resource");

	int32_t const dstRowPitch = TEXEL_SIZE * _srcWidth;
	int32_t const srcRowPitch = mapped.RowPitch;

	for (int32_t row = 0; row < _srcHeight; ++row)
		::memcpy(_data + row * dstRowPitch, (uint8_t const *)(mapped.pData) + row * srcRowPitch, dstRowPitch);

	d3d11ConcreteContext->Unmap(_stagingTexture, 0);

	return _data;
}

void TargetReader2D::shutdown()
{
	safeRelease(_stagingTexture);

	delete [] _data;
}

LUCID_GAL_D3D11_END