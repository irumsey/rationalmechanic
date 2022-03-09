#include "RenderTarget2D.h"
#include "System.h"
#include "Pipeline.h"
#include "Utility.h"
#include <lucid/core/Reader.h>
#include <lucid/core/Logger.h>

///
///
///
namespace /* anonymous */
{

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

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {

	RenderTarget2D *RenderTarget2D::create(FORMAT format, int32_t width, int32_t height)
	{
		return new ::lucid::gal::d3d11::RenderTarget2D(format, width, height);
	}

	RenderTarget2D *RenderTarget2D::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::RenderTarget2D(reader);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	RenderTarget2D::RenderTarget2D(FORMAT format, int32_t width, int32_t height)
		: _format(format)
		, _width(width), _height(height)
	{
		try
		{
			initialize();
		}
		catch (core::Error const &error)
		{
			core::log("ERR", error.what());
			shutdown();
			throw;
		}
	}

	RenderTarget2D::RenderTarget2D(::lucid::core::Reader &reader)
	{
		reader.read(&_format, sizeof(FORMAT));
		_width = reader.read<int32_t>();
		_height = reader.read<int32_t>();

		try
		{
			initialize();
		}
		catch (core::Error const &error)
		{
			core::log("ERR", error.what());
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
		descTexture.SampleDesc = { 1, 0, };
		descTexture.Usage = D3D11_USAGE_DEFAULT;
		descTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descTexture, nullptr, &_d3dTexture2D);
		GAL_VALIDATE_HRESULT(hResult, "unable to create render target");

		D3D11_RENDER_TARGET_VIEW_DESC descTargetView;
		::memset(&descTargetView, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		descTargetView.Format = descTexture.Format;
		descTargetView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		hResult = d3d11ConcreteDevice->CreateRenderTargetView(_d3dTexture2D, &descTargetView, &_d3dTargetView);
		GAL_VALIDATE_HRESULT(hResult, "unable to create render target");

		D3D11_SHADER_RESOURCE_VIEW_DESC descResourceView;
		::memset(&descResourceView, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		descResourceView.Format = descTexture.Format;
		descResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descResourceView.Texture2D.MipLevels = 1;

		hResult = d3d11ConcreteDevice->CreateShaderResourceView(_d3dTexture2D, &descResourceView, &_d3dResourceView);
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

}	///	d3d11
}	///	gal
}	///	lucid