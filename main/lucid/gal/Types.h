#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Matrix.h>

///
///
///
namespace lucid {
namespace gal {

	///
	///	gal does not require generic vertcies and matrices.
	///	these are convienience typedefs whose use is not required.
	///

	typedef ::lucid::math::Vector<float32_t,2> Vector2;
	typedef ::lucid::math::Vector<float32_t,3> Vector3;
	typedef ::lucid::math::Vector<float32_t,4> Vector4;

	typedef ::lucid::math::Quaternion<float32_t> Quaternion;

	typedef ::lucid::math::Matrix<float32_t,2,2> Matrix2x2;
	typedef ::lucid::math::Matrix<float32_t,3,3> Matrix3x3;
	typedef ::lucid::math::Matrix<float32_t,4,4> Matrix4x4;

	///	Color
	///
	///
	struct Color
	{
		float32_t r, g, b, a;

		Color()
			: r(1.f), g(1.f), b(1.f)
			, a(1.f)
		{
		}

		Color(float32_t r, float32_t g, float32_t b, float32_t a)
			: r(r), g(g), b(b)
			, a(a)
		{
		}
	};

	///	Viewport
	///
	///
	struct Viewport
	{
		int32_t x, y;
		int32_t width, height;
		float32_t znear, zfar;

		Viewport(int32_t x = 0, int32_t y = 0, int32_t width = 0, int32_t height = 0, float32_t znear = 0.f, float32_t zfar = 1.f)
			: x(x), y(y)
			, width(width), height(height)
			, znear(znear), zfar(zfar)
		{
		}
	};

}	///	gal
}	///	lucid
