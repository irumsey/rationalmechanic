#include "RenderState.h"
#include "System.h"
#include "Utility.h"
#include <lucid/core/Reader.h>

///
///
///
namespace /* anonymous */
{
	 
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

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {

	RenderState *RenderState::create(::lucid::core::Reader &reader)
	{
		return new ::lucid::gal::d3d11::RenderState(reader);
	}

}
}

namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///

	RasterState::RasterState(::lucid::core::Reader &reader)
	{
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

	RasterState::~RasterState()
	{
		safeRelease(d3dState);
	}

	///
	///
	///

	BlendState::BlendState(::lucid::core::Reader &reader)
	{
		colorWriteMask = reader.read<uint8_t>();
		enable = reader.read<bool>();

		srcBlend = readEnum(reader, d3dBlend);
		srcBlendAlpha = readEnum(reader, d3dBlend);

		dstBlend = readEnum(reader, d3dBlend);
		dstBlendAlpha = readEnum(reader, d3dBlend);

		D3D11_BLEND_DESC descBlend;
		::memset(&descBlend, 0, sizeof(descBlend));

		descBlend.AlphaToCoverageEnable = false;
		descBlend.IndependentBlendEnable = false;

		descBlend.RenderTarget[0].RenderTargetWriteMask = colorWriteMask;
		descBlend.RenderTarget[0].BlendEnable = enable;
		descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].SrcBlend = srcBlend;
		descBlend.RenderTarget[0].SrcBlendAlpha = srcBlendAlpha;
		descBlend.RenderTarget[0].DestBlend = dstBlend;
		descBlend.RenderTarget[0].DestBlendAlpha = dstBlendAlpha;

		HRESULT hResult = d3d11ConcreteDevice->CreateBlendState(&descBlend, &d3dState);
		GAL_VALIDATE_HRESULT(hResult, "unable to create blend state");
	}

	BlendState::~BlendState()
	{
		safeRelease(d3dState);
	}

	///
	///
	///

	DepthStencilState::DepthStencilState(::lucid::core::Reader &reader)
	{
		reader.read(depthEnable);
		writeMask = readEnum(reader, d3dDepthWriteMask);
		depthFunction = readEnum(reader, d3dCompare);

		reader.read(stencilEnable);
		stencilFunction = readEnum(reader, d3dCompare);

		stencilPass = readEnum(reader, d3dStencil);
		stencilFail = readEnum(reader, d3dStencil);
		stencilDepthFail = readEnum(reader, d3dStencil);

		reader.read(stencilReadMask);
		reader.read(stencilWriteMask);

		reader.read(stencilRef);

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

	RenderState::RenderState(::lucid::core::Reader &reader)
		: _rasterState(reader)
		, _blendState(reader)
		, _depthStencilState(reader)
	{
	}

}	///	d3d11
}	///	gal
}	///	lucid
