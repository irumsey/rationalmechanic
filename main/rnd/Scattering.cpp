#include "Scattering.h"
#include <lucid/gigl/Image.h>
#include <fstream>

namespace math = ::lucid::math;
namespace gigl = ::lucid::gigl;

namespace /* anonymous */ {

}

void Scattering::execute()
{
	_width = 512;
	_height = 512;
	_image  = new vector3_t[_width * _height];

	_lightDirection = math::normalize(vector3_t(1, 0, 0.5));
	_viewPosition = vector3_t(0, 0, 30.1f);
	_viewUp = vector3_t(0, 0, 1);
	_viewDirection = vector3_t(1, 0, 0);

	_atmosphere = sphere_t(vector3_t(), 33);
	_planet = sphere_t(vector3_t(), 30);

	float32_t fov = 0.25f * math::constants::pi<float32_t>();
	float32_t aspect = float32_t(_width) / float32_t(_height);

	vector3_t   eye = _viewPosition;
	vector3_t focus = _viewPosition + 10.f * _viewDirection;

	matrix4x4_t viewMatrix = math::look(eye, focus, _viewUp);
	matrix4x4_t projMatrix = math::perspective(fov, aspect, 1.f, 1000.f);
	matrix4x4_t viewProjMatrix = projMatrix * viewMatrix;
	matrix4x4_t invViewProjMatrix = math::inverse(viewProjMatrix);

	float32_t dx = 2.f / _width;
	float32_t dy = 2.f / _height;
	
	for (size_t row = 0; row < _height; ++row)
	{
		float32_t y = row * dy - 1.f;
		for (size_t col = 0; col < _width; ++col)
		{
			float32_t x = col * dx - 1.f;

			vector4_t ppsPosition = vector4_t(x, y, 0, 1);
			vector4_t worldPosition = invViewProjMatrix * ppsPosition;

			vector3_t direction = math::normalize(vector3_t(worldPosition.x, worldPosition.y, worldPosition.z) / worldPosition.w - eye);
			_image[row * _width + col] = trace(ray_t(eye, direction));
		}
	}

	writeImage("/temp/scattered1.tga");

	delete [] _image;
	_image = nullptr;
}

void Scattering::writeImage(std::string const &path)
{
	gigl::Image image(_width, _height);

	for (size_t row = 0; row < _height; ++row)
	{
		for (size_t col = 0; col < _width; ++col)
		{
			vector3_t const &hdr = _image[row * _width + col];

			gigl::Image::Pixel &pixel = image(col, row);

			pixel.r = uint8_t(255.f * math::clamp(hdr.x, 0.f, 1.f));
			pixel.g = uint8_t(255.f * math::clamp(hdr.y, 0.f, 1.f));
			pixel.b = uint8_t(255.f * math::clamp(hdr.z, 0.f, 1.f));
		}
	}

	image.write(path);
}
 