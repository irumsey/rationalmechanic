#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <cstdint>
#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/RenderState.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class RasterState
{
public:
	D3D11_FILL_MODE fill = D3D11_FILL_SOLID;
	D3D11_CULL_MODE cull = D3D11_CULL_FRONT;

	ID3D11RasterizerState *d3dState = nullptr;

	RasterState(LUCID_CORE::Reader &reader);

	virtual ~RasterState();

	LUCID_PREVENT_COPY(RasterState);
	LUCID_PREVENT_ASSIGNMENT(RasterState);
};

///
///
///
class BlendState
{
public:
	uint8_t colorWriteMask = 0xFF;

	bool enable = false;
	D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA;
	D3D11_BLEND srcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	D3D11_BLEND dstBlend = D3D11_BLEND_INV_SRC_ALPHA;
	D3D11_BLEND dstBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

	ID3D11BlendState *d3dState = nullptr;

	BlendState(LUCID_CORE::Reader &reader);

	virtual ~BlendState();

	LUCID_PREVENT_COPY(BlendState);
	LUCID_PREVENT_ASSIGNMENT(BlendState);
};

///
///
///
class DepthStencilState
{
public:
	bool depthEnable = false;
	D3D11_DEPTH_WRITE_MASK writeMask = D3D11_DEPTH_WRITE_MASK_ALL;
	D3D11_COMPARISON_FUNC depthFunction = D3D11_COMPARISON_LESS;

	bool stencilEnable = false;
	D3D11_COMPARISON_FUNC stencilFunction = D3D11_COMPARISON_EQUAL;
	D3D11_STENCIL_OP stencilPass = D3D11_STENCIL_OP_KEEP;
	D3D11_STENCIL_OP stencilFail = D3D11_STENCIL_OP_KEEP;
	D3D11_STENCIL_OP stencilDepthFail = D3D11_STENCIL_OP_KEEP;
	uint8_t stencilReadMask = 0xFF;
	uint8_t stencilWriteMask = 0xFF;

	uint8_t stencilRef = 0x00;

	ID3D11DepthStencilState *d3dState = nullptr;

	DepthStencilState(LUCID_CORE::Reader &reader);

	virtual ~DepthStencilState();

	LUCID_PREVENT_COPY(DepthStencilState);
	LUCID_PREVENT_ASSIGNMENT(DepthStencilState);
};

///
///
///
class RenderState : public LUCID_GAL::RenderState
{
public:
	RenderState(LUCID_CORE::Reader &reader);

	virtual ~RenderState() = default;

	ID3D11RasterizerState *d3dRasterizerState() const;

	ID3D11BlendState *d3dBlendState() const;

	ID3D11DepthStencilState *d3dDepthStencilState() const;

private:
	RasterState _rasterState;
	BlendState _blendState;
	DepthStencilState _depthStencilState;

	LUCID_PREVENT_COPY(RenderState);
	LUCID_PREVENT_ASSIGNMENT(RenderState);
};

inline ID3D11RasterizerState *RenderState::d3dRasterizerState() const
{
	return _rasterState.d3dState;
}

inline ID3D11BlendState *RenderState::d3dBlendState() const
{
	return _blendState.d3dState;
}

inline ID3D11DepthStencilState *RenderState::d3dDepthStencilState() const
{
	return _depthStencilState.d3dState;
}

LUCID_GAL_D3D11_END
