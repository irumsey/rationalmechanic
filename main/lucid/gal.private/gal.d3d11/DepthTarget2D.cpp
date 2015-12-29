#include "DepthTarget2D.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Reader.h>

///
///
///
namespace /* anonymous */
{

	/// ENUM LOOKUP
	DXGI_FORMAT const d3dFormat[] =
	{
		DXGI_FORMAT_D16_UNORM,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		DXGI_FORMAT_D32_FLOAT,
	};

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {

	DepthTarget2D *DepthTarget2D::create(FORMAT format, int32_t width, int32_t height)
	{
		return new ::lucid::gal::d3d11::DepthTarget2D(format, width, height);
	}

	DepthTarget2D *DepthTarget2D::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::DepthTarget2D(reader);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	DepthTarget2D::DepthTarget2D(FORMAT format, int32_t width, int32_t height)
		: _format(format)
		, _width(width), _height(height)
	{
		try
		{
			initialize();
		}
		catch (...)
		{
			shutdown();
			throw;
		}
	}

	DepthTarget2D::DepthTarget2D(::lucid::core::Reader &reader)
	{
		reader.read(&_format, sizeof(FORMAT));
		reader.read(_width);
		reader.read(_height);

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

}	///	d3d11
}	///	gal
}	///	lucid
