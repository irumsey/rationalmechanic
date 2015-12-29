#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <d3d11.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/TargetReader2D.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///	TargetReader2D
	///
	///	note: for now, the lifetime of the source target must include
	///	the lifetime if this...
	class TargetReader2D : public ::lucid::gal::TargetReader2D
	{
	public:
		TargetReader2D(::lucid::gal::RenderTarget2D const *target, int32_t width, int32_t height);

		virtual ~TargetReader2D();

		virtual int32_t width() const override;

		virtual int32_t height() const override;

		virtual uint8_t const *sample(int32_t x, int32_t y, int32_t width, int32_t height) const override;

	private:
		///	TBD: this is just for formats of 32 bits per texel.
		enum { TEXEL_SIZE = 4 };

		int32_t _srcWidth = 0;
		int32_t _srcHeight = 0;

		ID3D11Texture2D *_srcTexture = nullptr;

		int32_t _width = 0;
		int32_t _height = 0;

		ID3D11Texture2D *_stagingTexture = nullptr;

		uint8_t *_data = nullptr;

		void shutdown();

		LUCID_PREVENT_COPY(TargetReader2D);
		LUCID_PREVENT_ASSIGNMENT(TargetReader2D);
	};

	inline int32_t TargetReader2D::width() const
	{
		return _width;
	}

	inline int32_t TargetReader2D::height() const
	{
		return _height;
	}

}	///	d3d11
}	///	gal
}	///	lucid