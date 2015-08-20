#include "Pipeline.h"
#include "System.h"
#include "Program.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "DepthTarget2D.h"
#include "RenderTarget2D.h"
#include "Utility.h"
#include <lucid/core/Error.h>
#include <algorithm>

///
///
///
namespace /* anonymous */
{

	/// ENUM LOOKUP
	D3D11_PRIMITIVE_TOPOLOGY const d3dTopology[] =
	{
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};

}	///	anonymous

///
///
///
namespace lucid {
namespace gal {

	Pipeline &Pipeline::instance()
	{
		return ::lucid::gal::d3d11::Pipeline::instance();
	}

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	Pipeline::Pipeline()
	{
		std::fill(_d3dCurrentTargets, _d3dCurrentTargets + TARGET_MAXIMUM, nullptr);
	}

	Pipeline::~Pipeline()
	{
		shutdown();
	}

	void Pipeline::initialize(int32_t width, int32_t height, int32_t samples, ID3D11Device *d3dDevice_, ID3D11DeviceContext *d3dContext_, IDXGISwapChain *d3dChain_)
	{
		shutdown();

		_d3dDevice = d3dDevice_;
		_d3dContext = d3dContext_;
		_d3dChain = d3dChain_;

		_viewport = ::lucid::gal::Viewport(0, 0, width, height, 0.f, 1.f);

		HRESULT hResult = _d3dChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&_d3dTarget));
		GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline");

		hResult = _d3dDevice->CreateRenderTargetView(_d3dTarget, nullptr, &_d3dTargetView);
		GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline");

		D3D11_TEXTURE2D_DESC descDepth;
		::memset(&descDepth, 0, sizeof(D3D11_TEXTURE2D_DESC));

		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;

		descDepth.SampleDesc.Count = samples;
		descDepth.SampleDesc.Quality = (1 != samples) ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;

		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hResult = _d3dDevice->CreateTexture2D(&descDepth, nullptr, &_d3dDepth);
		GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline (1)");

		D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
		::memset(&descDepthView, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		descDepthView.Format = descDepth.Format;
		descDepthView.ViewDimension = (1 != samples) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;

		hResult = _d3dDevice->CreateDepthStencilView(_d3dDepth, &descDepthView, &_d3dDepthView);
		GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline (2)");

		viewport(::lucid::gal::Viewport(0, 0, width, height, 0.f, 1.f));
		restoreBackBuffer(true, true);
	}

	void Pipeline::shutdown()
	{
		safeRelease(_d3dTarget);
		safeRelease(_d3dTargetView);

		safeRelease(_d3dDepth);
		safeRelease(_d3dDepthView);

		_d3dChain = nullptr;
		_d3dContext = nullptr;
		_d3dDevice = nullptr;
	}

	void Pipeline::beginScene()
	{
		///	clear the per-frame statistics...
		_statistics.programChanges = 0;
		_statistics.targetChanges = 0;
		_statistics.drawCalls = 0;
	}

	void Pipeline::endScene()
	{
		_d3dChain->Present(1, 0);
	}

	void Pipeline::beginProgram(::lucid::gal::Program const *program)
	{
		updateTargets();

		_activeProgram = static_cast<::lucid::gal::d3d11::Program const *>(program);
		_activeProgram->onBegin();
	}

	void Pipeline::endProgram(::lucid::gal::Program const *program)
	{
		_activeProgram->onEnd();
		_activeProgram = nullptr;

		++_statistics.programChanges;
	}

	void Pipeline::beginGeometry(::lucid::gal::VertexFormat const *format)
	{
		updateTargets();

		::lucid::gal::d3d11::VertexFormat const *concrete = static_cast<VertexFormat const *>(format);
		_d3dContext->IASetInputLayout(concrete->d3dLayout());
	}

	void Pipeline::endGeometry(::lucid::gal::VertexFormat const *format)
	{
		///	TBD: is this ok ???

		ID3D11Buffer *buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };
		uint32_t dummy[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };

		_d3dContext->IASetInputLayout(nullptr);
		_d3dContext->IASetVertexBuffers(0, 8, buffer, dummy, dummy);
		_d3dContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}

	void Pipeline::setRenderTarget(int32_t index, ::lucid::gal::RenderTarget2D *renderTarget)
	{
		LUCID_VALIDATE((-1 < index) && (index < TARGET_MAXIMUM), "invalid index");

		::lucid::gal::d3d11::RenderTarget2D const *concrete = static_cast<gal::d3d11::RenderTarget2D const *>(renderTarget);
		_d3dCurrentTargets[index] = concrete ? concrete->d3dTargetView() : nullptr;

		_targetsChanged = true;

		++_statistics.targetChanges;
	}

	void Pipeline::setDepthTarget(::lucid::gal::DepthTarget2D *depthTarget)
	{
		::lucid::gal::d3d11::DepthTarget2D const *concrete = static_cast<::lucid::gal::d3d11::DepthTarget2D const *>(depthTarget);
		_d3dCurrentDepth = concrete ? concrete->d3dDepthStencilView() : nullptr;

		_targetsChanged = true;

		++_statistics.targetChanges;
	}

	void Pipeline::restoreBackBuffer(bool color, bool depth)
	{
		if (color)
		{
			std::fill(_d3dCurrentTargets, _d3dCurrentTargets + TARGET_MAXIMUM, nullptr);
			_d3dCurrentTargets[0] = _d3dTargetView;

			_targetsChanged = true;
		}

		if (depth)
		{
			_d3dCurrentDepth = _d3dDepthView;

			_targetsChanged = true;
		}

		++_statistics.targetChanges;
	}

	void Pipeline::viewport(::lucid::gal::Viewport const &viewport)
	{
		updateTargets();

		D3D11_VIEWPORT d3dViewport = { (float)viewport.x, (float)viewport.y, (float)viewport.width, (float)viewport.height, viewport.znear, viewport.zfar, };
		_d3dContext->RSSetViewports(1, &d3dViewport);
	}

	::lucid::gal::Viewport const &Pipeline::viewport() const
	{
		return _viewport;
	}

	void Pipeline::clear(bool clearTarget, bool clearDepth, bool clearStencil, ::lucid::gal::Color const &color, float32_t depth, uint8_t stencil)
	{
		updateTargets();

		for (int32_t i = 0; clearTarget && (i < TARGET_MAXIMUM); ++i)
		{
			ID3D11RenderTargetView *_d3dTarget = _d3dCurrentTargets[i];
			if (_d3dTarget)
			{
				_d3dContext->ClearRenderTargetView(_d3dTarget, reinterpret_cast<float const *>(&color));
			}
		}

		if (clearDepth && _d3dCurrentDepth)
		{
			if (clearStencil)
			{
				_d3dContext->ClearDepthStencilView(_d3dCurrentDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
			}
			else
			{
				_d3dContext->ClearDepthStencilView(_d3dCurrentDepth, D3D11_CLEAR_DEPTH, depth, stencil);
			}
		}
	}

	void Pipeline::setVertexStream(int32_t index, ::lucid::gal::VertexBuffer const *buffer, int32_t start)
	{
		updateTargets();

		::lucid::gal::d3d11::VertexBuffer const *concrete = static_cast<::lucid::gal::d3d11::VertexBuffer const *>(buffer);

		ID3D11Buffer *d3dBuffer = concrete->d3dBuffer();
		uint32_t stride = concrete->stride();
		uint32_t offset = start * stride;

		_d3dContext->IASetVertexBuffers(index, 1, &d3dBuffer, &stride, &offset);
	}

	void Pipeline::setIndexStream(::lucid::gal::IndexBuffer const *buffer)
	{
		updateTargets();

		::lucid::gal::d3d11::IndexBuffer const *concrete = static_cast<::lucid::gal::d3d11::IndexBuffer const *>(buffer);
		ID3D11Buffer *d3dBuffer = concrete->d3dBuffer();

		_d3dContext->IASetIndexBuffer(d3dBuffer, DXGI_FORMAT_R16_UINT, 0);
	}

	void Pipeline::draw(TOPOLOGY topology, int32_t vertexCount, int32_t indexCount, int32_t indexStart, int32_t indexOffset)
	{
		updateTargets();

		_activeProgram->onDraw();

		_d3dContext->IASetPrimitiveTopology(d3dTopology[topology]);
		_d3dContext->DrawIndexed(indexCount, indexStart, indexOffset);

		++_statistics.drawCalls;
	}

	void Pipeline::drawInstanced(TOPOLOGY topology, int32_t vertexCount, int32_t indexCount, int32_t instanceCount, int32_t indexStart, int32_t instanceStart, int32_t indexOffset)
	{
		updateTargets();

		_activeProgram->onDraw();

		_d3dContext->IASetPrimitiveTopology(d3dTopology[topology]);
		_d3dContext->DrawIndexedInstanced(indexCount, instanceCount, indexStart, indexOffset, instanceStart);

		++_statistics.drawCalls;
	}

	::lucid::gal::d3d11::Pipeline &Pipeline::instance()
	{
		static ::lucid::gal::d3d11::Pipeline theInstance;
		return theInstance;
	}

	void Pipeline::updateTargets()
	{
		if (_targetsChanged)
		{
			_d3dContext->OMSetRenderTargets(TARGET_MAXIMUM, _d3dCurrentTargets, _d3dCurrentDepth);
			_targetsChanged = false;
		}
	}

}	///	d3d11
}	///	gal
}	///	lucid