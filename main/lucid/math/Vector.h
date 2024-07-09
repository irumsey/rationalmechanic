#pragma once

#include <cassert>
#include <lucid/units/System.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Scalar.h>

LUCID_MATH_BEGIN

///	Vector<T, DIM, U>
///
///	general vector template
template<typename T, size_t DIM, typename U>
struct Vector
{
	T elements[DIM];

	Vector() = default;

	~Vector() = default;

	T &operator[](size_t i)
	{
		assert(i < DIM);
		return elements[i];
	}

	T const &operator[](size_t i) const
	{
		assert(i < DIM);
		return elements[i];
	}
};

///	Vector<T, 2, U>
///
///	2D specialization of Vector<>
///	providing (x,y) members.
template<typename T, typename U>
struct Vector<T, 2, U>
{
	union
	{
		T elements[2];
		struct { T x, y; };
	};

	Vector()
		: x(), y()
	{
	}

	Vector(T const &x, T const &y)
		: x(x), y(y)
	{
	}

	~Vector() = default;

	T &operator[](size_t i)
	{
		assert(i < 2);
		return elements[i];
	}

	T const &operator[](size_t i) const
	{
		assert(i < 2);
		return elements[i];
	}
};

///	Vector<T, 3, U>
///
///	3D specialization of Vector<>
///	providing (x,y,z) members.
template<typename T, typename U>
struct Vector<T, 3, U>
{
	union
	{
		T elements[3];
		struct { T x, y, z; };
	};

	Vector()
		: x(), y(), z()
	{
	}

	Vector(T const &x, T const &y, T const &z)
		: x(x), y(y), z(z)
	{
	}

	~Vector() = default;

	T &operator[](size_t i)
	{
		assert(i < 3);
		return elements[i];
	}

	T const &operator[](size_t i) const
	{
		assert(i < 3);
		return elements[i];
	}
};

///	Vector<T, 4, U>
///
///	4D specialization of Vector<>
///	providing (x,y,z,w) members.
template<typename T, typename U>
struct Vector<T, 4, U>
{
	union
	{
		T elements[4];
		struct { T x, y, z, w; };
	};

	Vector()
		: x(), y(), z(), w()
	{
	}

	Vector(T const &x, T const &y, T const &z, T const &w)
		: x(x), y(y), z(z), w(w)
	{
	}

	Vector(Vector<T, 3, U> const &v, T const &w)
		: x(v.x), y(v.y), z(v.z)
		, w(w)
	{
	}

	~Vector() = default;

	T &operator[](size_t i)
	{
		assert(i < 4);
		return elements[i];
	}

	T const &operator[](size_t i) const
	{
		assert(i < 4);
		return elements[i];
	}
};

LUCID_MATH_END

///
///	
///

template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_VECTOR(T, DIM, S, Q) operator-(LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, Q) result;

	result.elements[0] = -rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = -rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_VECTOR(T, DIM, S, Q) operator+(LUCID_VECTOR(T, DIM, S, Q) const &lhs, LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, Q) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] + rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_VECTOR(T, DIM, typename S, typename Q) operator-(LUCID_VECTOR(T, DIM, S, Q) const &lhs, LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, Q) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] - rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_VECTOR(T, DIM, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	result.elements[0] = lhs.elements[0] * rhs;
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] * rhs;

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_ADD(LQ, RQ)) operator*(LUCID_SCALAR(T, S, LQ) const &lhs, LUCID_VECTOR(T, DIM, S, RQ) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_ADD(LQ, RQ)) result;

	result.elements[0] = lhs * rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs * rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_SUB(LQ, RQ)) operator/(LUCID_VECTOR(T, DIM, S, LQ) const &lhs, LUCID_SCALAR(T, S, RQ) const &rhs)
{
	LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_SUB(LQ, RQ)) result;

	T const coeff = T(1) / rhs;

	result.elements[0] = coeff * lhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = coeff * lhs.elements[i];

	return result;
}

///
///
///

LUCID_MATH_BEGIN

///	interpolate
///
///	linear interpolation from a to b.
template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_VECTOR(T, DIM, S, Q) lerp(LUCID_SCALAR(T, S, LUCID_QUANTITY_PURE) const &t, LUCID_VECTOR(T, DIM, S, Q) const &a, LUCID_VECTOR(T, DIM, S, Q) const &b)
{
	return LUCID_VECTOR(T, DIM, S, Q)((b - a) * t + a);
}

///	dot
///
///	dot product
template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(LQ, RQ)) dot(LUCID_VECTOR(T, DIM, S, LQ) const &lhs, LUCID_VECTOR(T, DIM, S, RQ) const &rhs)
{
	LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(LQ, RQ)) result = lhs.elements[0] * rhs.elements[0];

	for (size_t i = 1; i < DIM; ++i)
		result = result + lhs.elements[i] * rhs.elements[i];

	return result;
}

///	lsq
///
///	length squared
template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_SCALAR(T, S, LUCID_QUANTITY_ADD(Q, Q)) lsq(LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	return dot(rhs, rhs);
}

///	len
///
///	length
template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) len(LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	return LUCID_SCALAR(T, S, Q)(std::sqrt(dot(rhs, rhs)));
}

///	normalize
///
///	normalize vector
template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_PURE) normalize(LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	return rhs / len(rhs);
}

///	cross
///
///	3D vector cross product
template<typename T, typename S, typename Q>
inline LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_ADD(Q, Q)) cross(LUCID_VECTOR(T, 3, S, Q) const &lhs, LUCID_VECTOR(T, 3, S, Q) const &rhs)
{
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_ADD(Q, Q)) result;

	result.x = lhs.y * rhs.z - rhs.y * lhs.z;
	result.y = rhs.x * lhs.z - lhs.x * rhs.z;
	result.z = lhs.x * rhs.y - rhs.x * lhs.y;
		
	return result;
}

LUCID_MATH_END
