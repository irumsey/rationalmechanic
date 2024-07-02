#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Texture2D.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_D3D11_BEGIN

///
///
///
class Texture2D : public ::lucid::gal::Texture2D
{
public:
	Texture2D(std::string const &path);

	Texture2D(::lucid::core::Reader &reader);

	virtual ~Texture2D();

	virtual int32_t width() const override;

	virtual int32_t height() const override;

	ID3D11ShaderResourceView *d3dResourceView() const;

private:
	int32_t _width = 0;
	int32_t _height = 0;

	ID3D11Resource *_d3dResource = nullptr;
	ID3D11ShaderResourceView *_d3dResourceView = nullptr;

	void initialize(std::string const &path);

	void shutdown();

	LUCID_PREVENT_COPY(Texture2D);
	LUCID_PREVENT_ASSIGNMENT(Texture2D);
};

inline int32_t Texture2D::width() const
{
	return _width;
}

inline int32_t Texture2D::height() const
{
	return _height;
}

inline ID3D11ShaderResourceView *Texture2D::d3dResourceView() const
{
	return _d3dResourceView;
}

LUCID_GAL_D3D11_END