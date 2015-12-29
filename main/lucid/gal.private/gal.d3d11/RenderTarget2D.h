#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/RenderTarget2D.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///
	class RenderTarget2D : public ::lucid::gal::RenderTarget2D
	{
	public:
		RenderTarget2D(FORMAT format, int32_t width, int32_t height);

		RenderTarget2D(::lucid::core::Reader &reader);

		virtual ~RenderTarget2D();

		virtual FORMAT format() const override;

		virtual int32_t width() const override;

		virtual int32_t height() const override;

		ID3D11Texture2D *d3dTexture() const;

		ID3D11RenderTargetView *d3dTargetView() const;

		ID3D11ShaderResourceView *d3dResourceView() const;

	private:
		FORMAT _format = FORMAT_UNDEFINED;
		int32_t _width = 0;
		int32_t _height = 0;

		ID3D11Texture2D *_d3dTexture2D = nullptr;
		ID3D11RenderTargetView *_d3dTargetView = nullptr;
		ID3D11ShaderResourceView *_d3dResourceView = nullptr;

		void initialize();

		void shutdown();

		LUCID_PREVENT_COPY(RenderTarget2D);
		LUCID_PREVENT_ASSIGNMENT(RenderTarget2D);
	};

	inline RenderTarget2D::FORMAT RenderTarget2D::format() const
	{
		return _format;
	}

	inline int32_t RenderTarget2D::width() const
	{
		return _width;
	}

	inline int32_t RenderTarget2D::height() const
	{
		return _height;
	}

	inline ID3D11Texture2D *RenderTarget2D::d3dTexture() const
	{
		return _d3dTexture2D;
	}

	inline ID3D11RenderTargetView *RenderTarget2D::d3dTargetView() const
	{
		return _d3dTargetView;
	}

	inline ID3D11ShaderResourceView *RenderTarget2D::d3dResourceView() const
	{
		return _d3dResourceView;
	}

}	///	d3d11
}	///	gal
}	///	lucid