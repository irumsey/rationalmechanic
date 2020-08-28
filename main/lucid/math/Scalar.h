#pragma once

#include <algorithm>
#include <cmath>

#ifdef min
#	undef min
#	undef max
#endif

namespace lucid {
namespace math {

	///
	///	equality tests
	///
	///	note: relies upon the "tolerance" constant
	///

	template<typename T> inline bool equ(T lhs, T rhs)
	{
		T delta = lhs - rhs;
		return (delta * delta) < constants::tol_tol<T>();
	}

	template<typename T> inline bool neq(T lhs, T rhs)
	{
		return !equ(lhs, rhs);
	}

	/// min / max
	///
	///

	template<typename T> inline T min(T const &x, T const &y)
	{
		return (x <= y) ? x : y;
	}

	template<typename T> inline T max(T const &x, T const &y)
	{
		return (x >= y) ? x : y;
	}

	///	interpolate
	///
	///	linear interpolation from a to b.
	template<typename T, typename U> inline U lerp(T const &t, U const &a, U const &b)
	{
		return (b - a) * t + a;
	}

	///	clamp
	///
	///	clamp value between values a and b
	///	where a < b
	template<typename T> inline T clamp(T const &x, T const &a, T const &b)
	{
		return std::min(std::max(a, x), b);
	}

	///	exclude
	///
	///	range of exclusion for the value x defined by a and b
	///	where a < b
	template<typename T> inline T exclude(T const &x, T const &a, T const &b)
	{
		if ((x < a) || (b < x))
			return x;

		if ((b - x) > (x - a))
			return a;

		return b;
	}

	///
	///	square root "wrapper"
	///

	template<typename T> inline T sqrt(T x)
	{
		return std::sqrt(x);
	}

	///
	///	cosine "wrappers"
	///

	template<typename T> inline T cos(T theta)
	{
		return std::cos(theta);
	}

	template<typename T> inline T acos(T x)
	{
		return std::acos(x);
	}

	///
	///	sine "wrappers"
	///

	template<typename T> inline T sin(T theta)
	{
		return std::sin(theta);
	}

	template<typename T> inline T asin(T x)
	{
		return std::asin(x);
	}

	///
	///	tangent "wrappers"
	///

	template<typename T> inline T tan(T theta)
	{
		return std::tan(theta);
	}

	template<typename T> inline T atan(T x)
	{
		return std::atan(x);
	}

	template<typename T> inline T atan2(T y, T x)
	{
		return std::atan2(y, x);
	}

}	///	math
}	///	lucid