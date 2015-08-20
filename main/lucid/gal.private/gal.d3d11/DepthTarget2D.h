#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/DepthTarget2D.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///
	class DepthTarget2D : public ::lucid::gal::DepthTarget2D
	{
	public:
		DepthTarget2D(FORMAT format, int32_t width, int32_t height);

		DepthTarget2D(::lucid::core::Reader &reader);

		virtual ~DepthTarget2D();

		virtual FORMAT format() const override;

		virtual int32_t width() const override;

		virtual int32_t height() const override;

		ID3D11DepthStencilView *d3dDepthStencilView() const;

	private:
		FORMAT _format = FORMAT_UNDEFINED;
		int32_t _width = 0;
		int32_t _height = 0;

		ID3D11Texture2D *_d3dTexture2D = nullptr;
		ID3D11DepthStencilView *_d3dDepthStencilView = nullptr;

		void initialize();

		void shutdown();

		LUCID_PREVENT_COPY(DepthTarget2D);
		LUCID_PREVENT_ASSIGNMENT(DepthTarget2D);
	};

	inline DepthTarget2D::FORMAT DepthTarget2D::format() const
	{
		return _format;
	}

	inline int32_t DepthTarget2D::width() const
	{
		return _width;
	}

	inline int32_t DepthTarget2D::height() const
	{
		return _height;
	}

	inline ID3D11DepthStencilView *DepthTarget2D::d3dDepthStencilView() const
	{
		return _d3dDepthStencilView;
	}

}	///	d3d11
}	///	gal
}	///	lucid