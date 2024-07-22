#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///	Quaternion<T>
///
///	Note: leaving the quaternion as "dimensionless"
template<typename T> struct Quaternion
{
	T x, y, z;
	T w;

	Quaternion() = default;

	Quaternion(T const &x, T const &y, T const &z, T const &w)
		: x(x), y(y), z(z)
		, w(w)
	{
	}

	~Quaternion() = default;

};

LUCID_MATH_END

template<typename T> inline LUCID_QUATERNION(T) operator-(LUCID_QUATERNION(T) const &rhs)
{
	return LUCID_QUATERNION(T)(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

template<typename T> inline LUCID_QUATERNION(T) operator+(LUCID_QUATERNION(T) const &lhs, LUCID_QUATERNION(T) const &rhs)
{
	return LUCID_QUATERNION(T)(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<typename T> inline LUCID_QUATERNION(T) operator-(LUCID_QUATERNION(T) const &lhs, LUCID_QUATERNION(T) const &rhs)
{
	return LUCID_QUATERNION(T)(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<typename T> inline LUCID_QUATERNION(T) operator*(LUCID_QUATERNION(T) const &lhs, T const &rhs)
{
	return LUCID_QUATERNION(T)(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template<typename T> inline LUCID_QUATERNION(T) operator*(T const &lhs, LUCID_QUATERNION(T) const &rhs)
{
	return LUCID_QUATERNION(T)(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
}

template<typename T> inline LUCID_QUATERNION(T) operator*(LUCID_QUATERNION(T) const &lhs, LUCID_QUATERNION(T) const &rhs)
{
	return LUCID_QUATERNION(T)
	(
		lhs.y * rhs.z - lhs.z * rhs.y + lhs.w * rhs.x + lhs.x * rhs.w,
		lhs.z * rhs.x - lhs.x * rhs.z + lhs.w * rhs.y + lhs.y * rhs.w,
		lhs.x * rhs.y - lhs.y * rhs.x + lhs.w * rhs.z + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	);
}

template<typename T> inline LUCID_QUATERNION(T) operator/(LUCID_QUATERNION(T) const &lhs, T const &rhs)
{
	T const coeff = T(1) / rhs;
	return LUCID_QUATERNION(T)(coeff * lhs.x, coeff * lhs.y, coeff * lhs.z, coeff * lhs.w);
}

LUCID_MATH_BEGIN

///	conjugate
///
///	returns the conjugate of the supplied quaternion
template<typename T> inline Quaternion<T> conjugate(Quaternion<T> const &rhs)
{
	return Quaternion<T>(-rhs.x, -rhs.y, -rhs.z, rhs.w);
}

///	dot
///
///	dot product
template<typename T> inline T dot(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

///	lsq
///
///	length squared
template<typename T> inline T lsq(Quaternion<T> const &rhs)
{
	return dot(rhs, rhs);
}

///	len
///
///	length
template<typename T> inline T len(Quaternion<T> const &rhs)
{
	return sqrt(lsq(rhs));
}

///	normalize
///
///	return normalized quaternion
template<typename T> inline Quaternion<T> normalize(Quaternion<T> const &rhs)
{
	return rhs / len(rhs);
}

///	rotateUsingAxis
///
///
template<typename T> inline Quaternion<T> rotateUsingAxis(LUCID_VECTOR(T, 3) const &axis, T const &theta)
{
	T hTheta = T(0.5) * theta;
	LUCID_VECTOR(T, 3) n = normalize(axis) * sin(hTheta);

	return Quaternion<T>(n.x, n.y, n.z, cos(hTheta));
}

///	transform direction
///
///
template<typename T> inline LUCID_VECTOR(T, 3) transformDirection(Quaternion<T> const q, LUCID_VECTOR(T, 3) const &r)
{
	Quaternion<T> u = q * Quaternion<T>(r.x, r.y, r.z, T(0)) * conjugate(q);

	return LUCID_VECTOR(T, 3)(u.x, u.y, u.z);
}

///	slerp
///
///	spherical interpolation.
template<typename T> inline LUCID_QUATERNION(T) slerp(T t, LUCID_QUATERNION(T) const &q1, LUCID_QUATERNION(T) const &q2)
{
	T dp = dot(q1, q2);
	LUCID_QUATERNION(T) q3 = q2;

	if (dp < T(0))
	{
		dp = -dp;
		q3 = -q2;
	}

	if (dp < T(0.95))
	{
		T theta = acos(dp);
		T coeff = T(1) / sin(theta);

		return coeff * (q1 * sin(theta * (T(1) - t)) + q3 * sin(theta * t));
	}

	return lerp(t, q1, q3);
}

///	slerpNoInvert
///
///
template<typename T> inline LUCID_QUATERNION(T) slerpNoInvert(T t, LUCID_QUATERNION(T) const &q1, LUCID_QUATERNION(T) const &q2)
{
	T dp = dot(q1, q2);

	if ((T(-0.95) < dp) && (dp < T(0.95)))
	{
		T theta = acos(dp);
		T coeff = T(1) / sin(theta);

		return coeff * (q1 * sin(theta * (T(1) - t)) + q2 * sin(theta * t));
	}

	return lerp(t, q1, q2);
}

///	exp
///
///	returns the exp value of the specified quaternion.
template<typename T> inline LUCID_QUATERNION(T) exp(LUCID_QUATERNION(T) const &rhs)
{
	T a = sqrt(rhs.x * rhs.x + rhs.y * rhs.y + rhs.z * rhs.z);
	LUCID_QUATERNION(T) result(0, 0, 0, cos(a));

	T sina = sin(a);
	if (sina > T(0))
	{
		T coeff = sina / a;

		result.x = coeff * rhs.x;
		result.y = coeff * rhs.y;
		result.z = coeff * rhs.z;
	}

	return result;
}

///	log
///
///	returns the log of the specified quaternion.
template<typename T> inline LUCID_QUATERNION(T) log(LUCID_QUATERNION(T) const &rhs)
{
	T a = acos(rhs.w);
	LUCID_QUATERNION(T) result(0, 0, 0, 0);

	T sina = sin(a);
	if (sina > T(0))
	{
		T coeff = a / sina;

		result.x = coeff * rhs.x;
		result.y = coeff * rhs.y;
		result.z = coeff * rhs.z;
	}

	return result;
}

///	squad
///
///	spherical spline interpolation.  s1 and s2 are computed using computeControl(...) below.
template<typename T> inline LUCID_QUATERNION(T) squad(T const &t, LUCID_QUATERNION(T) const &q1, LUCID_QUATERNION(T) const &q2, LUCID_QUATERNION(T) const &s1, LUCID_QUATERNION(T) const &s2)
{
	LUCID_QUATERNION(T) c = slerpNoInvert(t, q1, q2);
	LUCID_QUATERNION(T) d = slerpNoInvert(t, s1, s2);

	return slerpNoInvert(T(2) * t * (T(1) - t), c, d);
}

///	compute control
///
///	used to compute spline control values given: previous (q1), current (q2), and next (q3) values.
template<typename T> inline LUCID_QUATERNION(T) computeControl(LUCID_QUATERNION(T) const &q1, LUCID_QUATERNION(T) const &q2, LUCID_QUATERNION(T) const &q3)
{
	LUCID_QUATERNION(T) q(-q2.x, -q2.y, -q2.z, q2.w);

	return q2 * exp(T(-0.25) * (log(q * q1) + log(q * q3)));
}

LUCID_MATH_END
