#include "RenderTarget2D.h"
#include "System.h"
#include "Pipeline.h"
#include "Utility.h"
#include <lucid/core/Reader.h>
#include <lucid/core/Logger.h>

LUCID_ANONYMOUS_BEGIN

/// ENUM LOOKUP
DXGI_FORMAT const d3dFormat[] =
{
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R10G10B10A2_UINT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,

	DXGI_FORMAT_R32_UINT,
};

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

RenderTarget2D *RenderTarget2D::create(FORMAT format, int32_t width, int32_t height, int32_t samples)
{
	return new LUCID_GAL_D3D11::RenderTarget2D(format, width, height, samples);
}

RenderTarget2D *RenderTarget2D::create(LUCID_CORE::Reader &reader)
{
	return new LUCID_GAL_D3D11::RenderTarget2D(reader);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

RenderTarget2D::RenderTarget2D(FORMAT format, int32_t width, int32_t height, int32_t samples)
	: _format(format)
	, _width(width), _height(height)
	, _samples(samples)
{
	try
	{
		initialize();
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

RenderTarget2D::RenderTarget2D(LUCID_CORE::Reader &reader)
{
	reader.read(&_format, sizeof(FORMAT));
	_width = reader.read<int32_t>();
	_height = reader.read<int32_t>();
	_samples = 1;
	// TBD: add samples to bootstrap script...

	try
	{
		initialize();
	}
	catch (LUCID_CORE::Error const &error)
	{
		LUCID_CORE::log("ERR", error.what());
		shutdown();
		throw;
	}
}

RenderTarget2D::~RenderTarget2D()
{
	shutdown();
}

void RenderTarget2D::initialize()
{
	D3D11_TEXTURE2D_DESC descTexture;
	::memset(&descTexture, 0, sizeof(D3D11_TEXTURE2D_DESC));

	descTexture.Format = d3dFormat[_format];
	descTexture.Width = _width;
	descTexture.Height = _height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.SampleDesc.Count = _samples;
	descTexture.SampleDesc.Quality = (1 != _samples) ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descTexture, nullptr, &_d3dTexture2D);
	GAL_VALIDATE_HRESULT(hResult, "unable to create render target");

	hResult = d3d11ConcreteDevice->CreateRenderTargetView(_d3dTexture2D, nullptr, &_d3dTargetView);
	GAL_VALIDATE_HRESULT(hResult, "unable to create render target");

	hResult = d3d11ConcreteDevice->CreateShaderResourceView(_d3dTexture2D, nullptr, &_d3dResourceView);
	GAL_VALIDATE_HRESULT(hResult, "unable to create render target");

	++galConcreteStatistic(targets);
}

void RenderTarget2D::shutdown()
{
	safeRelease(_d3dTexture2D);
	safeRelease(_d3dTargetView);
	safeRelease(_d3dResourceView);

	--galConcreteStatistic(targets);
}

LUCID_GAL_D3D11_END