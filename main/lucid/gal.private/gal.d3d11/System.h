#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <lucid/gal/System.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

#define galConcreteSystem (LUCID_GAL_D3D11::System::instance())
#define d3d11ConcreteDevice (galConcreteSystem.d3dDevice())
#define d3d11ConcreteContext (galConcreteSystem.d3dContext())

LUCID_GAL_D3D11_BEGIN

///
///
///
class System : public ::lucid::gal::System
{
public:
	System() = default;

	virtual ~System();

	virtual void initialize(bool windowed, int32_t width, int32_t height, int32_t samples, void *window) override;

	virtual void shutdown() override;

	virtual void resize(int32_t width, int32_t height) override;

	virtual bool windowed() const override;

	virtual int32_t width() const override;

	virtual int32_t height() const override;

	virtual float32_t aspect() const override;

	ID3D11Device *d3dDevice() const;

	ID3D11DeviceContext *d3dContext() const;

	static ::lucid::gal::d3d11::System &instance();

private:
	bool _windowed = false;
	int _width = 0;
	int _height = 0;
	int _samples = 0;

	ID3D11Device *_d3dDevice = nullptr;
	ID3D11DeviceContext *_d3dContext = nullptr;
	IDXGISwapChain *_d3dChain = nullptr;

	LUCID_PREVENT_COPY(System);
	LUCID_PREVENT_ASSIGNMENT(System);
};

inline bool System::windowed() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return _windowed;
}

inline int32_t System::width() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return _width;
}

inline int32_t System::height() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return _height;
}

inline float32_t System::aspect() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return (float32_t) _width / _height;
}

inline ID3D11Device *System::d3dDevice() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return _d3dDevice;
}

inline ID3D11DeviceContext *System::d3dContext() const
{
	LUCID_VALIDATE(_d3dDevice, "use of uninitialized rendering system");
	return _d3dContext;
}

LUCID_GAL_D3D11_END