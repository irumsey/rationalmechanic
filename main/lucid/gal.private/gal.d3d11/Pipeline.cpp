#include "Pipeline.h"
#include "System.h"
#include "Program.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"
#include "DepthTarget2D.h"
#include "RenderTarget2D.h"
#include "Unordered2D.h"
#include "Utility.h"
#include <lucid/core/Error.h>
#include <algorithm>

LUCID_ANONYMOUS_BEGIN

/// ENUM LOOKUP
DXGI_FORMAT const d3dIndexFormat[] =
{
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT,
};

/// ENUM LOOKUP
D3D11_PRIMITIVE_TOPOLOGY const d3dTopology[] =
{
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
};

LUCID_ANONYMOUS_END

LUCID_GAL_BEGIN

Pipeline &Pipeline::instance()
{
	return ::lucid::gal::d3d11::Pipeline::instance();
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

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

	HRESULT hResult = _d3dChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&_d3dTarget));
	GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline");

	hResult = _d3dDevice->CreateRenderTargetView(_d3dTarget, nullptr, &_d3dTargetView);
	GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline");

	createDepthBuffer(width, height, samples);

	restoreBackBuffer(true, true);
	viewport(::lucid::gal::Viewport(0, 0, width, height, 0.f, 1.f));
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

void Pipeline::resize(int32_t width, int32_t height, int32_t samples)
{
	_d3dContext->OMSetRenderTargets(0, 0, 0);

	safeRelease(_d3dTarget);
	safeRelease(_d3dTargetView);

	HRESULT hResult = _d3dChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	GAL_VALIDATE_HRESULT(hResult, "unable to resize pipeline");

	hResult = _d3dChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&_d3dTarget));
	GAL_VALIDATE_HRESULT(hResult, "unable to resize pipeline");

	hResult = _d3dDevice->CreateRenderTargetView(_d3dTarget, nullptr, &_d3dTargetView);
	GAL_VALIDATE_HRESULT(hResult, "unable to resize pipeline");

	createDepthBuffer(width, height, samples);

	std::fill(_d3dCurrentTargets, _d3dCurrentTargets + TARGET_MAXIMUM, nullptr);
	_d3dCurrentDepth = nullptr;

	restoreBackBuffer(true, true);
	viewport(::lucid::gal::Viewport(0, 0, width, height, 0.f, 1.f));
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
	ID3D11Buffer *buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };
	uint32_t dummy[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };

	_d3dContext->IASetInputLayout(nullptr);
	_d3dContext->IASetVertexBuffers(0, 8, buffer, dummy, dummy);
	_d3dContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

void Pipeline::setUnorderedTarget(::lucid::gal::Unordered2D *unordered)
{
	::lucid::gal::d3d11::Unordered2D *concrete = static_cast<::lucid::gal::d3d11::Unordered2D *>(unordered);
	_d3dCurrentUnordered = concrete ? concrete->d3dUnorderedView() : nullptr;

	_targetsChanged = true;
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

void Pipeline::restoreBackBuffer(bool color, bool depth, bool unordered)
{
	if (color)
	{
		std::fill(_d3dCurrentTargets, _d3dCurrentTargets + TARGET_MAXIMUM, nullptr);
		_d3dCurrentTargets[0] = _d3dTargetView;
	}

	if (depth)
	{
		_d3dCurrentDepth = _d3dDepthView;
	}

	if (unordered)
	{
		_d3dCurrentUnordered = nullptr;
	}

	_targetsChanged = color | depth | unordered;

	++_statistics.targetChanges;
}

void Pipeline::updateTargets()
{
	if (_targetsChanged)
	{
		_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(TARGET_MAXIMUM, _d3dCurrentTargets, _d3dCurrentDepth, UNORDERED_SLOT, 1, &_d3dCurrentUnordered, 0);
		_targetsChanged = false;
	}
}

void Pipeline::viewport(::lucid::gal::Viewport const &viewport)
{
	updateTargets();

	D3D11_VIEWPORT d3dViewport = { (float)viewport.x, (float)viewport.y, (float)viewport.width, (float)viewport.height, viewport.znear, viewport.zfar, };
	_d3dContext->RSSetViewports(1, &d3dViewport);

	_viewport = viewport;
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
		ID3D11RenderTargetView *d3dTarget = _d3dCurrentTargets[i];
		if (d3dTarget)
		{
			_d3dContext->ClearRenderTargetView(d3dTarget, reinterpret_cast<float const *>(&color));
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

	_d3dContext->IASetIndexBuffer(d3dBuffer, d3dIndexFormat[buffer->format()], 0);
}

void Pipeline::draw(TOPOLOGY topology, int32_t vertexCount)
{
	updateTargets();

	_activeProgram->onDraw();

	_d3dContext->IASetPrimitiveTopology(d3dTopology[topology]);
	_d3dContext->Draw(vertexCount, 0);

	++_statistics.drawCalls;
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

void Pipeline::createDepthBuffer(int32_t width, int32_t height, int32_t samples)
{
	D3D11_TEXTURE2D_DESC descDepth;
	::memset(&descDepth, 0, sizeof(D3D11_TEXTURE2D_DESC));

	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;

	descDepth.SampleDesc.Count = samples;
	descDepth.SampleDesc.Quality = (1 != samples) ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;

	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	HRESULT hResult = _d3dDevice->CreateTexture2D(&descDepth, nullptr, &_d3dDepth);
	GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline (1)");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
	::memset(&descDepthView, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	descDepthView.Format = descDepth.Format;
	descDepthView.ViewDimension = (1 != samples) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;

	hResult = _d3dDevice->CreateDepthStencilView(_d3dDepth, &descDepthView, &_d3dDepthView);
	GAL_VALIDATE_HRESULT(hResult, "unable to initialize pipeline (2)");
}

LUCID_GAL_D3D11_END
