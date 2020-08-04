#pragma once

#include <string>
#include <lucid/core/Types.h>

///
///
///

#define LUCID_MATH_CONSTANT(name,value)				\
	template<class T> static T name()				\
	{												\
		static_assert(false, "bad cast");			\
		return T();									\
	}												\
	template<> static float32_t name()				\
	{												\
		static float32_t const result = value##F;	\
		return result;								\
	}												\
	template<> static float64_t name()				\
	{												\
		static float64_t const result = value;		\
		return result;								\
	}												\
	template<> static float128_t name()				\
	{												\
		static float128_t const result = value##L;	\
		return result;								\
	}

///
///
///
namespace lucid {
namespace math {
namespace constants {

		LUCID_MATH_CONSTANT(      zero, 0.0)
		LUCID_MATH_CONSTANT(       one, 1.0)
		LUCID_MATH_CONSTANT(      half, 0.5)

		LUCID_MATH_CONSTANT(    two_pi, 6.283185307179586476925286766559)
		LUCID_MATH_CONSTANT(        pi, 3.141592653589793238462643383279)
		LUCID_MATH_CONSTANT(   half_pi, 1.570796326794896619231321691639)
		LUCID_MATH_CONSTANT(quarter_pi, 0.785398163397448309615660845819)

		LUCID_MATH_CONSTANT(         e, 2.718281828459045235360287471352)

		LUCID_MATH_CONSTANT(   pos_inf,  1e38)
		LUCID_MATH_CONSTANT(   neg_inf, -1e38)

		LUCID_MATH_CONSTANT(       tol, 0.0000001)
		LUCID_MATH_CONSTANT(   tol_tol, 0.00000000000001)

}	///	constants
}	///	math
}	///	lucid
