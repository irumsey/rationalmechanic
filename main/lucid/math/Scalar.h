#pragma once

#include <cmath>
#include <lucid/core/Types.h>
#include <lucid/math/Constants.h>

///
///
///
namespace lucid {
namespace math {

	///
	///	equality tests
	///
	///	note: relies upon the "tolerance" constant
	///

	template<class T> inline bool equ(T lhs, T rhs)
	{
		T delta = lhs - rhs;
		return (delta * delta) < constants::tol_tol<T>();
	}
	
	template<class T> inline bool neq(T lhs, T rhs)
	{
		return !equ(lhs, rhs);
	}

	///
	///	square root "wrapper"
	///

	template<class T> inline T sqrt(T x)
	{
		return std::sqrt(x);
	}
	
	///
	///	cosine "wrappers"
	///

	template<class T> inline T cos(T theta)
	{
		return std::cos(theta);
	}

	template<class T> inline T acos(T x)
	{
		return std::acos(x);
	}

	///
	///	sine "wrappers"
	///

	template<class T> inline T sin(T theta)
	{
		return std::sin(theta);
	}

	template<class T> inline T asin(T x)
	{
		return std::asin(x);
	}

	///
	///	tangent "wrappers"
	///

	template<class T> inline T tan(T theta)
	{
		return std::tan(theta);
	}

	template<class T> inline T atan(T x)
	{
		return std::atan(x);
	}

	template<class T> inline T atan2(T y, T x)
	{
		return std::atan2(y, x);
	}

}	///	math
}	///	lucid
