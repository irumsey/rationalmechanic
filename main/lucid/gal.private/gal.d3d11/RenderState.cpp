#include "RenderState.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Unserializer.h>

LUCID_ANONYMOUS_BEGIN

/// ENUM LOOKUP
D3D11_FILL_MODE const d3dFill[] =
{
	D3D11_FILL_SOLID,
	D3D11_FILL_WIREFRAME,
};

/// ENUM LOOKUP
D3D11_CULL_MODE const d3dCull[] =
{
    D3D11_CULL_FRONT,
    D3D11_CULL_BACK,
    D3D11_CULL_NONE,
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
    
/// ENUM LOOKUP
D3D11_DEPTH_WRITE_MASK const d3dDepthWriteMask[] =
{
	D3D11_DEPTH_WRITE_MASK_ZERO,
	D3D11_DEPTH_WRITE_MASK_ALL,
};

/// ENUM LOOKUP
D3D11_STENCIL_OP const d3dStencil[] =
{
    D3D11_STENCIL_OP_KEEP,
    D3D11_STENCIL_OP_ZERO,
    D3D11_STENCIL_OP_REPLACE,
    D3D11_STENCIL_OP_INCR_SAT,
    D3D11_STENCIL_OP_INCR,
    D3D11_STENCIL_OP_DECR_SAT,
    D3D11_STENCIL_OP_DECR,
    D3D11_STENCIL_OP_INVERT,
};
    
/// ENUM LOOKUP
D3D11_BLEND const d3dBlend[] =
{
    D3D11_BLEND_ONE,
    D3D11_BLEND_SRC_COLOR,
    D3D11_BLEND_INV_SRC_COLOR,
    D3D11_BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR,
    D3D11_BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA,
    D3D11_BLEND_DEST_ALPHA,
    D3D11_BLEND_INV_DEST_ALPHA,
};

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

RenderState *RenderState::create(LUCID_CORE::Unserializer &reader)
{
	return new LUCID_GAL_D3D11::RenderState(reader);
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN


RasterState::~RasterState()
{
	safeRelease(d3dState);
}

void RasterState::read(LUCID_CORE::Unserializer &reader)
{
	safeRelease(d3dState);

	///
	/// read the data...
	///

	fill = readEnum(reader, d3dFill);
	cull = readEnum(reader, d3dCull);

	bool enableMultisample = reader.read<bool>();

	///
	///	create the d3d11 resource...
	///

	D3D11_RASTERIZER_DESC descState;
	::memset(&descState, 0, sizeof(D3D11_RASTERIZER_DESC));

	descState.FillMode = fill;
	descState.CullMode = cull;
	descState.FrontCounterClockwise = false;
	descState.DepthBias = 0;
	descState.DepthBiasClamp = 0;
	descState.SlopeScaledDepthBias = 0.f;
	descState.DepthClipEnable = true;
	descState.ScissorEnable = false;
	descState.MultisampleEnable = enableMultisample;
	descState.AntialiasedLineEnable = false;

	HRESULT hResult = d3d11ConcreteDevice->CreateRasterizerState(&descState, &d3dState);
	GAL_VALIDATE_HRESULT(hResult, "unable to create rasterizer state");
}

///
///
///

BlendState::~BlendState()
{
	safeRelease(d3dState);
}

void BlendState::read(LUCID_CORE::Unserializer &reader)
{
	safeRelease(d3dState);

	colorWriteMask = reader.read<uint8_t>();
	enable = reader.read<bool>();

	srcBlend = readEnum(reader, d3dBlend);
	srcBlendAlpha = readEnum(reader, d3dBlend);

	dstBlend = readEnum(reader, d3dBlend);
	dstBlendAlpha = readEnum(reader, d3dBlend);

	D3D11_BLEND_DESC descBlend;
	::memset(&descBlend, 0, sizeof(descBlend));

	descBlend.AlphaToCoverageEnable = false;
	descBlend.IndependentBlendEnable = true;

	// TBD: data drive blending for all render target channels
	// for now, just allow targets 0 and 1 to alpha blend
	for (size_t i = 0; i < 8; ++i)
	{
		descBlend.RenderTarget[i].RenderTargetWriteMask = (i < 2) ? colorWriteMask : 0x0f;
		descBlend.RenderTarget[i].BlendEnable = (i < 2) ? enable : false;

		descBlend.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[i].SrcBlend = srcBlend;
		descBlend.RenderTarget[i].SrcBlendAlpha = srcBlendAlpha;
		descBlend.RenderTarget[i].DestBlend = dstBlend;
		descBlend.RenderTarget[i].DestBlendAlpha = dstBlendAlpha;
	}

	HRESULT hResult = d3d11ConcreteDevice->CreateBlendState(&descBlend, &d3dState);
	GAL_VALIDATE_HRESULT(hResult, "unable to create blend state");
}

///
///
///

void DepthStencilState::read(LUCID_CORE::Unserializer &reader)
{
	safeRelease(d3dState);

	depthEnable = reader.read<bool>();
	writeMask = readEnum(reader, d3dDepthWriteMask);
	depthFunction = readEnum(reader, d3dCompare);

	stencilEnable = reader.read<bool>();
	stencilFunction = readEnum(reader, d3dCompare);

	stencilPass = readEnum(reader, d3dStencil);
	stencilFail = readEnum(reader, d3dStencil);
	stencilDepthFail = readEnum(reader, d3dStencil);

	stencilReadMask = reader.read<uint8_t>();
	stencilWriteMask = reader.read<uint8_t>();

	stencilRef = reader.read<uint8_t>();

	D3D11_DEPTH_STENCIL_DESC descState;
	::memset(&descState, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));

	descState.DepthEnable = depthEnable;
	descState.DepthWriteMask = writeMask;
	descState.DepthFunc = depthFunction;

	descState.StencilEnable = stencilEnable;

	descState.StencilReadMask = stencilReadMask;
	descState.StencilWriteMask = stencilWriteMask;

	descState.FrontFace.StencilDepthFailOp = stencilDepthFail;
	descState.FrontFace.StencilFailOp = stencilFail;
	descState.FrontFace.StencilPassOp = stencilPass;
	descState.FrontFace.StencilFunc = stencilFunction;

	descState.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	descState.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	descState.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	descState.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hResult = d3d11ConcreteDevice->CreateDepthStencilState(&descState, &d3dState);
	GAL_VALIDATE_HRESULT(hResult, "unable to create depth stencil state");
}

DepthStencilState::~DepthStencilState()
{
	safeRelease(d3dState);
}

///
///
///

RenderState::RenderState(LUCID_CORE::Unserializer &reader)
{
	reader.member_begin();
	_rasterState.read(reader);
	reader.member_end();

	reader.member_begin();
	_blendState.read(reader);
	reader.member_end();

	reader.member_begin();
	_depthStencilState.read(reader);
	reader.member_end();
}

LUCID_GAL_D3D11_END
