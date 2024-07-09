#include "System.h"
#include "Pipeline.h"
#include "Utility.h"

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

LUCID_GAL_BEGIN

System &System::instance()
{
	return LUCID_GAL_D3D11::System::instance();
}

LUCID_GAL_END

LUCID_GAL_D3D11_BEGIN

System::~System()
{
	shutdown();
}

void System::initialize(bool windowed, int32_t width, int32_t height, int32_t samples, void *window)
{
	shutdown();

	_windowed = windowed;
	_width = width;
	_height = height;
	_samples = samples;

	DXGI_SWAP_CHAIN_DESC descChain;
	::memset(&descChain, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	descChain.BufferCount = 1;
	descChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descChain.BufferDesc.Width = _width;
	descChain.BufferDesc.Height = _height;
	descChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descChain.OutputWindow = (HWND)window;
	descChain.Windowed = _windowed;

	descChain.SampleDesc.Count = _samples;
	descChain.SampleDesc.Quality = (1 != samples) ? D3D11_STANDARD_MULTISAMPLE_PATTERN : 0;

	descChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	uint32_t flags = 0;

#if defined(GAL_ENABLE_NATIVE_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hResult = ::D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, 0, 0, D3D11_SDK_VERSION, &descChain, &_d3dChain, &_d3dDevice, nullptr, &_d3dContext);
	GAL_VALIDATE_HRESULT(hResult, "unable to initialize gal system");

	galConcretePipeline.initialize(_width, _height, _samples, _d3dDevice, _d3dContext, _d3dChain);
}

void System::shutdown()
{
	galConcretePipeline.shutdown();

	safeRelease(_d3dContext);
	safeRelease(_d3dChain);
	safeRelease(_d3dDevice);
}

void System::resize(int32_t width, int32_t height)
{
	_width = width;
	_height = height;

	galConcretePipeline.resize(_width, _height, _samples);
}

LUCID_GAL_D3D11::System &System::instance()
{
	static LUCID_GAL_D3D11::System theInstance;
	return theInstance;
}

LUCID_GAL_D3D11_END
