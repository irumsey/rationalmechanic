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
		try
		{
			initialize(width, height);
		}
		catch (...)
		{
			shutdown();
			throw;
		}
	}

	Image::Image(std::string const &path)
	{
		try
		{
			read(path);
		}
		catch (...)
		{
			shutdown();
			throw;
		}
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

	void Image::read(std::string const &path)
	{
		std::ifstream stream(path, std::ios::binary);
		LUCID_VALIDATE(stream.is_open(), "unable to open image file, '" + path + "', for reading");

		uint8_t header[HEADER_SIZE];

		stream.read((char *)(header), HEADER_SIZE);
		LUCID_VALIDATE(PIXEL_DEPTH == header[16], "invalid pixel depth specified in file, '" + path + "', for reading");

		size_t width = ((size_t)header[13] << 8) | header[12];
		size_t height = ((size_t)header[15] << 8) | header[14];

		initialize(width, height);

		stream.read((char *)(_data), dataSize());
	}

	void Image::write(std::string const &path) const
	{
		std::ofstream stream(path, std::ios::binary);
		LUCID_VALIDATE(stream.is_open(), "unable to open image file, '" + path + "', for writing");

		uint8_t header[HEADER_SIZE];
		::memset(header, 0, HEADER_SIZE);

		header[ 2] = 2;
		header[12] = 0xff &   _width      ;
		header[13] = 0xff & ( _width >> 8);
		header[14] = 0xff &  _height      ;
		header[15] = 0xff & (_height >> 8);
		header[16] = uint8_t(PIXEL_DEPTH);
		header[17] = 0;

		stream.write((char const *)(header), HEADER_SIZE);
		stream.write((char const *)(_data), dataSize());

		stream.close();
	}

}	///	gigl
}	///	lucid
