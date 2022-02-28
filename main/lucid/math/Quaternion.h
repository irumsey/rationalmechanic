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
	};

}	///	math
}	///	lucid

///	quaternion shorthand for the following operator overloads
///
///	note: relies upon push/pop macro pragmas
#pragma push_macro("QUATERNION")
#define QUATERNION(T) lucid::math::Quaternion<T>

template<typename T> inline QUATERNION(T) operator-(QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

template<typename T> inline QUATERNION(T) operator+(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<typename T> inline QUATERNION(T) operator-(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<typename T> inline QUATERNION(T) operator*(QUATERNION(T) const &lhs, T const &rhs)
{
	return QUATERNION(T)(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template<typename T> inline QUATERNION(T) operator*(T const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
}

template<typename T> inline QUATERNION(T) operator*(QUATERNION(T) const &lhs, QUATERNION(T) const &rhs)
{
	return QUATERNION(T)
	(
		lhs.y * rhs.z - lhs.z * rhs.y + lhs.w * rhs.x + lhs.x * rhs.w,
		lhs.z * rhs.x - lhs.x * rhs.z + lhs.w * rhs.y + lhs.y * rhs.w,
		lhs.x * rhs.y - lhs.y * rhs.x + lhs.w * rhs.z + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	);
}

template<typename T> inline QUATERNION(T) operator/(QUATERNION(T) const &lhs, T const &rhs)
{
	T const coeff = ::lucid::math::constants::one<T>() / rhs;
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

	///	equ and neq
	///
	///	equality tests
	
	template<typename T> inline bool equ(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
	{
		return lsq(lhs - rhs) < constants::tol_tol<T>();
	}

	template<typename T> inline bool neq(Quaternion<T> const &lhs, Quaternion<T> const &rhs)
	{
		return !equ(lhs, rhs);
	}

	///	rotateUsingAxis
	///
	///
	template<typename T> Quaternion<T> rotateUsingAxis(Vector<T,3> const &axis, T theta)
	{
		T hTheta = constants::half<T>() * theta;
		Vector<T,3> n = normalize(axis) * sin(hTheta);

		return Quaternion<T>(n.x, n.y, n.z, cos(hTheta));
	}

	///	transform direction
	///
	///
	template<typename T> inline Vector<T,3> transformDirection(Quaternion<T> const q, Vector<T,3> const &r)
	{
		Quaternion<T> u = q * Quaternion<T>(r.x, r.y, r.z, constants::zero<T>()) * conjugate(q);

		return Vector<T,3>(u.x, u.y, u.z);
	}

	///	slerp
	///
	///	spherical interpolation.
	template<typename T> inline Quaternion<T> slerp(T t, Quaternion<T> const &q1, Quaternion<T> const &q2)
	{
		T dp = dot(q1, q2);
		Quaternion<T> q3 = q2;

		if (dp < constants::zero<T>())
		{
			dp = -dp;
			q3 = -q2;
		}

		if (dp < T(0.95))
		{
			T theta = acos(dp);
			T coeff = constants::one<T>() / sin(theta);

			return coeff * (q1 * sin(theta * (constants::one<T>() - t)) + q3 * sin(theta * t));
		}

		return lerp(t, q1, q3);
	}

	///	slerpNoInvert
	///
	///
	template<typename T> inline Quaternion<T> slerpNoInvert(T t, Quaternion<T> const &q1, Quaternion<T> const &q2)
	{
		T dp = dot(q1, q2);

		if ((T(-0.95) < dp) && (dp < T(0.95)))
		{
			T theta = acos(dp);
			T coeff = constants::one<T>() / sin(theta);

			return coeff * (q1 * sin(theta * (constants::one<T>() - t)) + q2 * sin(theta * t));
		}

		return lerp(t, q1, q2);
	}

	///	exp
	///
	///	returns the exp value of the specified quaternion.
	template<typename T> inline Quaternion<T> exp(Quaternion<T> const &rhs)
	{
		T a = sqrt(rhs.x * rhs.x + rhs.y * rhs.y + rhs.z * rhs.z);
		T sina = sin(a);
		T cosa = cos(a);

		Quaternion<T> result(0, 0, 0, cosa);
		if (a > constants::zero<T>())
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
	template<typename T> inline Quaternion<T> log(Quaternion<T> const &rhs)
	{
		T a = acos(rhs.w);
		T sina = sin(a);

		Quaternion<T> result(0, 0, 0, 0);
		if (sina > constants::zero<T>())
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
	template<typename T> inline Quaternion<T> squad(T t, Quaternion<T> const &q1, Quaternion<T> const &q2, Quaternion<T> const &s1, Quaternion<T> const &s2)
	{
		Quaternion<T> c = slerpNoInvert(t, q1, q2);
		Quaternion<T> d = slerpNoInvert(t, s1, s2);

		return slerpNoInvert(constants::two<T>() * t * (constants::one<T>() - t), c, d);
	}

	///	compute control
	///
	///	used to compute spline control values given: previous (q1), current (q2), and next (q3) values.
	template<typename T> inline Quaternion<T> computeControl(Quaternion<T> const &q1, Quaternion<T> const &q2, Quaternion<T> const &q3)
	{
		Quaternion<T> q(-q2.x, -q2.y, -q2.z, q2.w);

		return q2 * exp(T(-0.25) * (log(q * q1) + log(q * q3)));
	}

}	///	math
}	///	lucid
