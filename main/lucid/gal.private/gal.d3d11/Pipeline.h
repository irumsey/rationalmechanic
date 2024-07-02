#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/gal/Statistics.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

#define galConcretePipeline (LUCID_GAL_D3D11::Pipeline::instance())
#define galConcreteStatistic(name) (galConcretePipeline.statistics().name)

LUCID_GAL_D3D11_BEGIN


class Program;

///
///
///
class Pipeline : public ::lucid::gal::Pipeline
{
public:
	Pipeline();

	virtual ~Pipeline();

	void initialize(int32_t width, int32_t height, int32_t samples, ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext, IDXGISwapChain *d3dChain);

	void shutdown();

	void resize(int32_t width, int32_t height, int32_t samples);

	virtual Statistics &statistics() override;

	virtual Statistics const &statistics() const override;

	virtual void beginScene() override;

	virtual void endScene() override;

	virtual void beginProgram(::lucid::gal::Program const *program) override;

	virtual void endProgram(::lucid::gal::Program const *program) override;

	virtual void beginGeometry(::lucid::gal::VertexFormat const *format) override;

	virtual void endGeometry(::lucid::gal::VertexFormat const *format) override;

	virtual void setUnorderedTarget(::lucid::gal::Unordered2D *unordered) override;

	virtual void setRenderTarget(int32_t index, ::lucid::gal::RenderTarget2D *renderTarget) override;

	virtual void setDepthTarget(::lucid::gal::DepthTarget2D *depthtarget) override;

	virtual void restoreBackBuffer(bool color = true, bool depth = true, bool unordered = true) override;

	virtual void updateTargets() override;

	virtual void viewport(::lucid::gal::Viewport const &viewport) override;

	virtual ::lucid::gal::Viewport const &viewport() const override;

	virtual void clear(bool clearTarget = true, bool clearDepth = true, bool clearStencil = true, ::lucid::gal::Color const &color = ::lucid::gal::Color(), float32_t depth = 1.f, uint8_t stencil = 0x00) override;

	virtual void setVertexStream(int32_t index, ::lucid::gal::VertexBuffer const *buffer, int32_t start = 0) override;

	virtual void setIndexStream(::lucid::gal::IndexBuffer const *buffer) override;

	virtual void draw(TOPOLOGY topology, int32_t vertexCount) override;

	virtual void draw(TOPOLOGY topology, int32_t vertexCount, int32_t indexCount, int32_t indexStart = 0, int32_t indexOffset = 0) override;

	virtual void drawInstanced(TOPOLOGY topology, int32_t vertexCount, int32_t indexCount, int32_t instanceCount, int32_t indexStart = 0, int32_t instanceStart = 0, int32_t indexOffset = 0) override;

	static ::lucid::gal::d3d11::Pipeline &instance();

private:
	static int32_t const TARGET_MAXIMUM = 4;
	static int32_t const UNORDERED_SLOT = TARGET_MAXIMUM;

	Statistics _statistics;
	::lucid::gal::Viewport _viewport;

	ID3D11Device *_d3dDevice = nullptr;
	ID3D11DeviceContext *_d3dContext = nullptr;
	IDXGISwapChain *_d3dChain = nullptr;

	ID3D11Texture2D *_d3dTarget = nullptr;
	ID3D11RenderTargetView *_d3dTargetView = nullptr;

	ID3D11Texture2D *_d3dDepth = nullptr;
	ID3D11DepthStencilView *_d3dDepthView = nullptr;

	ID3D11UnorderedAccessView *_d3dCurrentUnordered = nullptr;
	ID3D11RenderTargetView *_d3dCurrentTargets[TARGET_MAXIMUM];
	ID3D11DepthStencilView *_d3dCurrentDepth = nullptr;
	bool _targetsChanged = true;

	::lucid::gal::d3d11::Program const *_activeProgram = nullptr;

	void createDepthBuffer(int32_t width, int32_t height, int32_t samples);

	LUCID_PREVENT_COPY(Pipeline);
	LUCID_PREVENT_ASSIGNMENT(Pipeline);
};

inline Statistics &Pipeline::statistics()
{
	return _statistics;
}

inline Statistics const &Pipeline::statistics() const
{
	return _statistics;
}

LUCID_GAL_D3D11_END