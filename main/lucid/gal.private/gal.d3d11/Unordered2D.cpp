#include "Unordered2D.h"
#include "System.h"
#include "Pipeline.h"
#include "Utility.h"
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <vector>

///
///
///
namespace lucid {
namespace gal {

	Unordered2D *Unordered2D::create(int32_t width, int32_t height)
	{
		return new ::lucid::gal::d3d11::Unordered2D(width, height);
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	Unordered2D::Unordered2D(int32_t width, int32_t height)
		: _width(width)
		, _height(height)
	{
		try
		{
			///
			///	create texture...
			///

			D3D11_TEXTURE2D_DESC descTexture;
			::memset(&descTexture, 0, sizeof(D3D11_TEXTURE2D_DESC));

			descTexture.Width = width;
			descTexture.Height = height;
			descTexture.MipLevels = 1;
			descTexture.ArraySize = 1;
			descTexture.SampleDesc.Count = 1;
			descTexture.SampleDesc.Quality = 0;
			descTexture.Usage = D3D11_USAGE_DEFAULT;
			descTexture.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			descTexture.Format = DXGI_FORMAT_R32_UINT;

			HRESULT hResult = d3d11ConcreteDevice->CreateTexture2D(&descTexture, nullptr, &_d3dTexture);
			GAL_VALIDATE_HRESULT(hResult, "unable to create unordered access texture");

			///
			///	create unordered access view...
			///

			D3D11_UNORDERED_ACCESS_VIEW_DESC descUnordered;
			::memset(&descUnordered, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

			descUnordered.Format = DXGI_FORMAT_R32_UINT;
			descUnordered.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			descUnordered.Texture2D.MipSlice = 0;

			hResult = d3d11ConcreteDevice->CreateUnorderedAccessView(_d3dTexture, &descUnordered, &_d3dUnorderedView);
			GAL_VALIDATE_HRESULT(hResult, "unable to create unordered access texture");

			///
			///	create resource view...
			///

			D3D11_SHADER_RESOURCE_VIEW_DESC descResource;
			::memset(&descResource, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			descResource.Format = DXGI_FORMAT_R32_UINT;
			descResource.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			descResource.Texture2D.MipLevels = 1;

			hResult = d3d11ConcreteDevice->CreateShaderResourceView(_d3dTexture, &descResource, &_d3dResourceView);
			GAL_VALIDATE_HRESULT(hResult, "unable to create unordered access texture");

			///
			///	create the staging texture for reading back the data...
			///

			D3D11_TEXTURE2D_DESC descReader;
			::memset(&descReader, 0, sizeof(D3D11_TEXTURE2D_DESC));

			descReader.Usage = D3D11_USAGE_STAGING;
			descReader.Format = DXGI_FORMAT_R32_UINT;
			descReader.Width = width;
			descReader.Height = height;
			descReader.ArraySize = 1;
			descReader.SampleDesc = descTexture.SampleDesc;
			descReader.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

			hResult = d3d11ConcreteDevice->CreateTexture2D(&descReader, nullptr, &_d3dStaging);
			GAL_VALIDATE_HRESULT(hResult, "unable to create unordered access texture");

			///
			///	create the array...
			///

			_data = new uint32_t [width * height];
		}
		catch (core::Error const &error)
		{
			core::log("ERR", error.what());
			shutdown();
			throw;
		}
	}

	Unordered2D::~Unordered2D()
	{
		shutdown();
	}

	void Unordered2D::clear()
	{
		uint32_t const zeros[] = { 0, 0, 0, 0, };
		d3d11ConcreteContext->ClearUnorderedAccessViewUint(_d3dUnorderedView, zeros);
	}

	void Unordered2D::sync()
	{
		D3D11_BOX box = { 0, 0, 0, UINT(_width), UINT(_height), 1, };
		d3d11ConcreteContext->CopySubresourceRegion(_d3dStaging, 0, 0, 0, 0, _d3dTexture, 0, &box);

		D3D11_MAPPED_SUBRESOURCE mapped = {};

		HRESULT hResult = d3d11ConcreteContext->Map(_d3dStaging, 0, D3D11_MAP_READ, 0, &mapped);
		GAL_VALIDATE_HRESULT(hResult, "unable to map staging resource");

		::memcpy(_data, mapped.pData, sizeof(uint32_t) * _width * _height);

		d3d11ConcreteContext->Unmap(_d3dStaging, 0);
	}

	void Unordered2D::shutdown()
	{
		delete [] _data;

		safeRelease(_d3dStaging);

		safeRelease(_d3dResourceView);
		safeRelease(_d3dUnorderedView);

		safeRelease(_d3dResource);

		safeRelease(_d3dTexture);

		++galConcreteStatistic(textures);
	}

}	///	d3d11
}	///	gal
}	///	lucid