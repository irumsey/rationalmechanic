#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Fixed.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/AABB.h>

namespace lucid {
namespace orbit {

	// typedef float32_t scalar_t;
	typedef float64_t scalar_t;
	// typedef float128_t scalar_t;
	// typedef ::lucid::math::Fixed<256, 32> scalar_t;
	typedef ::lucid::math::Vector<scalar_t, 2> vector2_t;
	typedef ::lucid::math::Vector<scalar_t, 3> vector3_t;
	typedef ::lucid::math::Matrix<scalar_t, 3, 3> matrix3x3_t;
	typedef ::lucid::math::AABB<scalar_t, 3> aabb3_t;

}	///	orbit
}	///	lucid
