#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Unordered2D.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>

LUCID_GAL_D3D11_BEGIN

///	Unordered2D
///
///
class Unordered2D : public ::lucid::gal::Unordered2D
{
public:
	Unordered2D(int32_t width, int32_t height);

	virtual ~Unordered2D();

	virtual int32_t width() const override;

	virtual int32_t height() const override;

	virtual void clear() override;

	virtual void sync() override;

	virtual uint32_t at(int32_t row, int32_t col) const override;

	ID3D11UnorderedAccessView *d3dUnorderedView() const;

	ID3D11ShaderResourceView *d3dResourceView() const;

private:
	int32_t _width = 0;
	int32_t _height = 0;

	ID3D11Texture2D *_d3dTexture = nullptr;

	ID3D11Resource *_d3dResource = nullptr;
		
	ID3D11UnorderedAccessView *_d3dUnorderedView = nullptr;
	ID3D11ShaderResourceView *_d3dResourceView = nullptr;

	ID3D11Texture2D *_d3dStaging = nullptr;
	uint32_t *_data = nullptr;

	void shutdown();

	LUCID_PREVENT_COPY(Unordered2D);
	LUCID_PREVENT_ASSIGNMENT(Unordered2D);
};

inline int32_t Unordered2D::width() const
{
	return _width;
}

inline int32_t Unordered2D::height() const
{
	return _height;
}

inline uint32_t Unordered2D::at(int32_t row, int32_t col) const
{
	return _data[row * _width + col];
}

inline ID3D11UnorderedAccessView *Unordered2D::d3dUnorderedView() const
{
	return _d3dUnorderedView;
}

inline ID3D11ShaderResourceView *Unordered2D::d3dResourceView() const
{
	return _d3dResourceView;
}

LUCID_GAL_D3D11_END