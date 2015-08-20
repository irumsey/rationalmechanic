#pragma once

#include <lucid/math/Constants.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

///
///
///
namespace lucid {
namespace math {

	///	Quaternion<T>
	///
	///	templated quaternion
	template<class T> struct Quaternion
	{
		T x, y, z;
		T w;

		Quaternion() = default;

		Quaternion(T const &x, T const &y, T const &z, T const &w)
			: x(x), y(y), z(z)
			, w(w)
		{
		}
	};

}	///	math
}	///	lucid

///	quaternion shorthand for the following operator overloads
///
///	note: relies upon push/pop macro pragmas
#pragma push_macro("QUATERNION")
#define QUATERNION(T) lucid::math::Quaternion<T>

template<class T> inline QUATERNION(T) operator-(QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

template<class T> inline QUATERNION(T) operator+(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<class T> inline QUATERNION(T) operator-(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<class T> inline QUATERNION(T) operator*(QUATERNION(T) const &lhs, T const &rhs)
{
	return QUATERNION(T)(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template<class T> inline QUATERNION(T) operator*(T const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
}

template<class T> inline QUATERNION(T) operator*(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)
	(
		lhs.y * rhs.z - lhs.z * rhs.y + lhs.w * rhs.x + lhs.x * rhs.w,
		lhs.z * rhs.x - lhs.x * rhs.z + lhs.w * rhs.y + lhs.y * rhs.w,
		lhs.x * rhs.y - lhs.y * rhs.x + lhs.w * rhs.z + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	);
}

template<class T> inline QUATERNION(T) operator/(QUATERNION(T) const &lhs, T const &rhs)
{
	T const coeff = constants::one<T>() / rhs;
	return QUATERNION(T)(coeff * lhs.x, coeff * lhs.y, coeff * lhs.z, coeff * lhs.w);
}

///
///	remove the shorthand
///
#undef QUATERNION
#pragma pop_macro("QUATERNION")

///
///
///
namespace lucid {
namespace math {

	///	conjugate
	///
	///	returns the conjugate of the supplied quaternion
	template<class T> inline Quaternion<T> conjugate(Quaternion<T> const &rhs)
	{
		return Quaternion<T>(-rhs.x, -rhs.y, -rhs.z, rhs.w);
	}

	///	dot
	///
	///	dot product
	template<class T> inline T dot(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	///	lsq
	///
	///	length squared
	template<class T> inline T lsq(Quaternion<T> const &rhs)
	{
		return dot(rhs, rhs);
	}

	///	len
	///
	///	length
	template<class T> inline T len(Quaternion<T> const &rhs)
	{
		return sqrt(lsq(rhs));
	}

	///	normalize
	///
	///	return normalized quaternion
	template<class T> inline Quaternion<T> normalize(Quaternion<T> const &rhs)
	{
		return rhs / len(rhs);
	}

	///	equ and neq
	///
	///	equality tests
	
	template<class T> inline bool equ(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
	{
		return lsq(lhs - rhs) < constants::tol_tol<T>();
	}

	template<class T> inline bool neq(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
	{
		return !equ(lhs, rhs);
	}

	///	rotateUsingAxis
	///
	///
	template<class T> Quaternion<T> rotateUsingAxis(Vector<T,3> const &axis, T theta)
	{
		T hTheta = constants::half<T>() * theta;
		Vector<T,3> n = normalize(axis) * sin(hTheta);

		return Quaternion<T>(n.x, n.y, n.z, cos(hTheta));
	}

	///	transform direction
	///
	///
	template<class T> inline Vector<T,3> transformDirection(Quaternion<T> const q, Vector<T,3> const &r)
	{
		Quaternion<T> u = q * Quaternion<T>(r.x, r.y, r.z, constants::zero<T>()) * conjugate(q);
		return Vector<T,3>(u.x, u.y, u.z);
	}

}	///	math
}	///	lucid
