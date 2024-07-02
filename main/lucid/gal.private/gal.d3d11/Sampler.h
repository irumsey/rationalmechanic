#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <cstdint>
#include <string>
#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class SamplerState
{
public:
	D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	D3D11_TEXTURE_ADDRESS_MODE addru = D3D11_TEXTURE_ADDRESS_CLAMP;
	D3D11_TEXTURE_ADDRESS_MODE addrv = D3D11_TEXTURE_ADDRESS_CLAMP;
	D3D11_TEXTURE_ADDRESS_MODE addrw = D3D11_TEXTURE_ADDRESS_CLAMP;
	float lodMipBias = 0.f;
	uint32_t maxAnisotropy = 1;
	D3D11_COMPARISON_FUNC compare = D3D11_COMPARISON_NEVER;
	::lucid::gal::Color borderColor = ::lucid::gal::Color(1, 1, 1, 1);
	float lodMin = -3.4e38F;
	float lodMax = 3.4e38F;

	ID3D11SamplerState *d3dState = nullptr;

	SamplerState(::lucid::core::Reader &reader);

	virtual ~SamplerState();

	LUCID_PREVENT_COPY(SamplerState);
	LUCID_PREVENT_ASSIGNMENT(SamplerState);
};

///
///
///
class Sampler
{
public:
	Sampler *next = nullptr;

	std::string name = "<unknown>";

	int32_t vsPosition = -1;
	int32_t gsPosition = -1;
	int32_t psPosition = -1;

	SamplerState state;

	Sampler(std::string const &name, ::lucid::core::Reader &reader);

	virtual ~Sampler();

	LUCID_PREVENT_COPY(Sampler);
	LUCID_PREVENT_ASSIGNMENT(Sampler);
};

LUCID_GAL_D3D11_END