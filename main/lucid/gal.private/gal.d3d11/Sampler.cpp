#include "Sampler.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Reader.h>

///
///
///
namespace /* anonymous */
{

	/// ENUM LOOKUP
	D3D11_FILTER const d3dFilter[] =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_ANISOTROPIC,
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
		D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_COMPARISON_ANISOTROPIC,
		D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MINIMUM_ANISOTROPIC,
		D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR,
		D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT,
		D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MAXIMUM_ANISOTROPIC,
	};

	/// ENUM LOOKUP
	D3D11_TEXTURE_ADDRESS_MODE const d3dAddress[] =
	{
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,
	};

	/// ENUM LOOKUP
	D3D11_COMPARISON_FUNC const d3dCompare[] =
	{
		D3D11_COMPARISON_NEVER,
		D3D11_COMPARISON_LESS,
		D3D11_COMPARISON_EQUAL,
		D3D11_COMPARISON_LESS_EQUAL,
		D3D11_COMPARISON_GREATER,
		D3D11_COMPARISON_NOT_EQUAL,
		D3D11_COMPARISON_GREATER_EQUAL,
		D3D11_COMPARISON_ALWAYS,
	};

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///

	SamplerState::SamplerState(::lucid::core::Reader &reader)
	{
		///
		///	read the data...
		///

		filter = readEnum(reader, d3dFilter);

		addru = readEnum(reader, d3dAddress);
		addrv = readEnum(reader, d3dAddress);
		addrw = readEnum(reader, d3dAddress);

		lodMipBias = reader.read<float>();
		maxAnisotropy = reader.read<uint32_t>();

		compare = readEnum(reader, d3dCompare);

		reader.read(&borderColor, sizeof(::lucid::gal::Color));

		lodMin = reader.read<float>();
		lodMax = reader.read<float>();

		///
		///	create the resource...
		///

		D3D11_SAMPLER_DESC descSampler;
		::memset(&descSampler, 0, sizeof(D3D11_SAMPLER_DESC));

		descSampler.Filter = filter;

		descSampler.AddressU = addru;
		descSampler.AddressV = addrv;
		descSampler.AddressW = addrw;

		descSampler.MipLODBias = lodMipBias;
		descSampler.MaxAnisotropy = maxAnisotropy;

		descSampler.ComparisonFunc = compare;

		::memcpy(&(descSampler.BorderColor), &borderColor, 4 * sizeof(float));

		descSampler.MinLOD = lodMin;
		descSampler.MaxLOD = lodMax;

		HRESULT hResult = d3d11ConcreteDevice->CreateSamplerState(&descSampler, &d3dState);
		GAL_VALIDATE_HRESULT(hResult, "unable to create sampler state");
	}

	SamplerState::~SamplerState()
	{
		safeRelease(d3dState);
	}

	///
	///
	///

	Sampler::Sampler(std::string const &name, ::lucid::core::Reader &reader)
		: name(name)
		, state(reader)
	{
	}

	Sampler::~Sampler()
	{
		delete next;
	}

}	///	d3d11
}	///	gal
}	///	lucid