#pragma once

#include <algorithm>
#include <cmath>
#include <lucid/units/System.h>
#include <lucid/math/Defines.h>

#ifdef min
#	undef min
#	undef max
#endif

LUCID_MATH_BEGIN

///	Scalar
///
///	a value with a unit of measure
template<typename T, typename U>
struct Scalar
{
	T value;

	Scalar()
		: value()
	{
	}

	Scalar(T const &rhs)
		: value(rhs)
	{
	}

	~Scalar() = default;

	operator T const &() const
	{
		return value;
	}

};

LUCID_MATH_END

template<typename T, typename S, typename Q>
inline bool operator<(LUCID_SCALAR(T, S, Q) const &lhs, LUCID_SCALAR(T, S, Q) const &rhs)
{
	return lhs.value < rhs.value;
}

template<typename T, typename S, typename Q>
inline bool operator>(LUCID_SCALAR(T, S, Q) const &lhs, LUCID_SCALAR(T, S, Q) const &rhs)
{
	return lhs.value > rhs.value;
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) operator-(LUCID_SCALAR(T, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, Q)(-rhs.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) operator+(LUCID_SCALAR(T, S, Q) const &lhs, LUCID_SCALAR(T, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, Q)(lhs.value + rhs.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) operator-(LUCID_SCALAR(T, S, Q) const &lhs, LUCID_SCALAR(T, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, Q)(lhs.value - rhs.value);
}

template<typename T, typename S, typename LQ, typename RQ>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_SCALAR(T, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(LQ, RQ))(lhs.value * rhs.value);
}

template<typename T, typename S, typename LQ, typename RQ>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_SUB(LQ, RQ)) operator/(LUCID_SCALAR(T, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_SUB(LQ, RQ))(lhs.value / rhs.value);
}

LUCID_MATH_BEGIN

/// min / max
/// 
/// 

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) min(LUCID_SCALAR(T, S, Q) const &x, LUCID_SCALAR(T, S, Q) const &y)
{
	return LUCID_SCALAR(T, S, Q)((x.value <= y.value) ? x.value : y.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) max(LUCID_SCALAR(T, S, Q) const &x, LUCID_SCALAR(T, S, Q) const &y)
{
	return LUCID_SCALAR(T, S, Q)((x.value >= y.value) ? x.value : y.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) abs(LUCID_SCALAR(T, S, Q) const &rhs)
{
	return std::abs(rhs.value);
}

///	interpolate
///
///	linear interpolation from a to b.
template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) lerp(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &t, LUCID_SCALAR(T, S, Q) const &a, LUCID_SCALAR(T, S, Q) const &b)
{
	return LUCID_SCALAR(T, S, Q)((b.value - a.value) * t.value + a.value);
}

///	clamp
///
///	clamp value between values a and b
///	where a < b
template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) clamp(LUCID_SCALAR(T, S, Q) const &x, LUCID_SCALAR(T, S, Q) const &a, LUCID_SCALAR(T, S, Q) const &b)
{
	return LUCID_SCALAR(T, S, Q)(std::min(std::max(a.value, x.value), b.value));
}

///	exclude
///
///	range of exclusion for the value x defined by a and b
///	where a < b
template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) exclude(LUCID_SCALAR(T, S, Q) const &x, LUCID_SCALAR(T, S, Q) const &a, LUCID_SCALAR(T, S, Q) const &b)
{
	if ((x.value < a.value) || (b.value < x.value))
		return x;

	if ((b.value - x.value) > (x.value - a.value))
		return a;

	return LUCID_SCALAR(T, S, Q)(b.value);
}

///
///
///
template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_NEG(Q)) inv(LUCID_SCALAR(T, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_NEG(Q))(T(1) / rhs.value);
}

///
///
///
template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_HALF(Q)) sqrt(LUCID_SCALAR(T, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_HALF(Q))(std::sqrt(rhs.value));
}

///
///	cosine "wrappers"
///

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) cos(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &theta)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::cos(theta.value));
}

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) acos(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &x)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::acos(x.value));
}

///
///	sine "wrappers"
///

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) sin(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &theta)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::sin(theta.value));
}

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) asin(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &x)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::asin(x.value));
}

///
///	tangent "wrappers"
///

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) tan(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &theta)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::tan(theta.value));
}

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) atan(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &x)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::atan(x.value));
}

template<typename T, typename S>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) atan2(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &y, LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &x)
{
	return LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE)(std::atan2(y.value, x.value));
}

LUCID_MATH_END
