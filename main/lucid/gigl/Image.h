#pragma once

#include <string>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

namespace lucid {
namespace gigl {

	///	Image
	///
	///	Utility for writing a basic (unmapped) TGA file.
	class Image
	{
	public:

#pragma pack(push)
#pragma pack(1)
		///	Pixel
		///
		///
		struct Pixel
		{
			uint8_t b = 0;
			uint8_t g = 0;
			uint8_t r = 0;

			Pixel() = default;

			Pixel(uint8_t r, uint8_t g, uint8_t b)
				: r(r), g(g), b(b)
			{
			}
		};
#pragma pack(pop)

		Image();

		Image(size_t width, size_t height);

		virtual ~Image();

		void initialize(size_t width, size_t height);

		void shutdown();

		size_t width() const;

		size_t height() const;

		Pixel const &operator()(size_t x, size_t y) const;

		Pixel &operator()(size_t x, size_t y);

		Pixel const &at(size_t x, size_t y) const;

		Pixel &at(size_t x, size_t y);

		void write(std::string const &path) const;

	private:
		size_t const  PIXEL_SIZE = sizeof(Pixel);
		size_t const PIXEL_DEPTH = 8 * PIXEL_SIZE;

		size_t _width = 0;
		size_t _height = 0;
		Pixel *_data = 0;

		size_t dataSize() const;

		LUCID_PREVENT_COPY(Image);
		LUCID_PREVENT_ASSIGNMENT(Image);
	};

	inline size_t Image::width() const
	{
		return _width;
	}

	inline size_t Image::height() const
	{
		return _height;
	}

	inline Image::Pixel const &Image::operator()(size_t x, size_t y) const
	{
		return at(x, y);
	}

	inline Image::Pixel &Image::operator()(size_t x, size_t y)
	{
		return at(x, y);
	}

	inline Image::Pixel const &Image::at(size_t x, size_t y) const
	{
		return _data[ y * _width + x ];
	}

	inline Image::Pixel &Image::at(size_t x, size_t y)
	{
		return _data[ y * _width + x ];
	}

	inline size_t Image::dataSize() const
	{
		return _width * _height * PIXEL_SIZE;
	}

}	///	gigl
}	///	lucid
