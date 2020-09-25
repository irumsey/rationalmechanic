#include "Image.h"
#include <lucid/math/Algorithm.h>
#include <lucid/core/Error.h>
#include <fstream>

namespace math = ::lucid::math;

namespace lucid {
namespace gigl {

	Image::Image()
	{
	}

	Image::Image(size_t width, size_t height)
	{
		initialize(width, height);
	}

	Image::~Image()
	{
		shutdown();
	}

	void Image::initialize(size_t width, size_t height)
	{
		shutdown();

		_width = width;
		_height = height;

		_data = new Pixel [ _width * _height ];
	}

	void Image::shutdown()
	{
		delete [] _data;
		_data = nullptr;

		_width = 0;
		_height = 0;
	}

	void Image::write(std::string const &path) const
	{
		std::ofstream stream(path, std::ios::binary);
		LUCID_VALIDATE(stream.is_open(), "unable to open image file, '" + path + "', for writing");

		uint8_t header[18];
		::memset(header, 0, 18);

		header[ 2] = 2;
		header[12] = 0xff &   _width      ;
		header[13] = 0xff & ( _width >> 8);
		header[14] = 0xff &  _height      ;
		header[15] = 0xff & (_height >> 8);
		header[16] = uint8_t(PIXEL_DEPTH);
		header[17] = 0;

		stream.write((char const *)(header), 18);
		stream.write((char const *)(_data), dataSize());

		stream.close();
	}

}	///	gigl
}	///	lucid
