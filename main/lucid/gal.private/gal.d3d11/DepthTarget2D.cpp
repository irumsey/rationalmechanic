#include "DepthTarget2D.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Reader.h>
#include <lucid/core/Logger.h>

LUCID_ANONYMOUS_BEGIN

/// ENUM LOOKUP
DXGI_FORMAT const d3dFormat[] =
{
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT,
};

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

	DepthTarget2D *DepthTarget2D::create(FORMAT format, int32_t width, int32_t height)
	{
		return new LUCID_GAL_D3D11::DepthTarget2D(format, width, height);
	}

	DepthTarget2D *DepthTarget2D::create(LUCID_CORE::Reader &reader)
	{
		return new LUCID_GAL_D3D11::DepthTarget2D(reader);
	}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

DepthTarget2D::DepthTarget2D(FORMAT format, int32_t width, int32_t height)
	: _format(format)
	, _width(width), _height(height)
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

DepthTarget2D::DepthTarget2D(LUCID_CORE::Reader &reader)
{
	reader.read(&_format, sizeof(FORMAT));
	_width = reader.read<int32_t>();
	_height = reader.read<int32_t>();

	initialize();
}

DepthTarget2D::~DepthTarget2D()
{
	shutdown();
}

void DepthTarget2D::initialize()
{
	D3D11_TEXTURE2D_DESC descTexture;
	::memset(&descTexture, 0, sizeof(D3D11_TEXTURE2D_DESC));

	descTexture.Format = d3dFormat[_format];
	descTexture.Width = _width;
	descTexture.Height = _height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.SampleDesc = { 1, 0, };
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descTexture, nullptr, &_d3dTexture2D);
	GAL_VALIDATE_HRESULT(hResult, "unable to create depth texture");

	hResult = d3d11ConcreteDevice->CreateDepthStencilView(_d3dTexture2D, nullptr, &_d3dDepthStencilView);
	GAL_VALIDATE_HRESULT(hResult, "unable to create depth view");
}

void DepthTarget2D::shutdown()
{
	safeRelease(_d3dTexture2D);
	safeRelease(_d3dDepthStencilView);
}

LUCID_GAL_D3D11_END