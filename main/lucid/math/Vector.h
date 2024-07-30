#pragma once

#include <cassert>
#include <cmath>
#include <lucid/math/Defines.h>

LUCID_MATH_BEGIN

///	Vector<T, DIM>
///
///	general vector template
template<typename T, size_t DIM> struct Vector
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

///	Vector<T, 2>
///
///	2D specialization of Vector<>
///	providing (x,y) members.
template<typename T> struct Vector<T, 2>
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

///	Vector<T, 3>
///
///	3D specialization of Vector<>
///	providing (x,y,z) members.
template<typename T> struct Vector<T, 3>
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

	Vector(Vector<T, 2> const &r, T const &z)
		: x(r.x), y(r.y), z(z)
	{
	}

	Vector(T const &x, T const &y, T const &z)
		: x(x), y(y), z(z)
	{
	}

	~Vector() = default;

	Vector<T, 2> xy() const
	{
		return Vector<T, 2>(x, y);
	}

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

///	Vector<T, 4>
///
///	4D specialization of Vector<>
///	providing (x,y,z,w) members.
template<typename T> struct Vector<T, 4>
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

	Vector(Vector<T, 3> const &r, T const &w)
		: x(r.x), y(r.y), z(r.z), w(w)
	{
	}

	Vector(T const &x, T const &y, T const &z, T const &w)
		: x(x), y(y), z(z), w(w)
	{
	}

	~Vector() = default;

	Vector<T, 2> xy() const
	{
		return Vector<T, 2>(x, y);
	}

	Vector<T, 3> xyz() const
	{
		return Vector<T, 3>(x, y, z);
	}

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

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator-(LUCID_VECTOR(T, DIM) const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

	result.elements[0] = -rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = -rhs.elements[i];

	return result;
}

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator+(LUCID_VECTOR(T, DIM) const &lhs, LUCID_VECTOR(T, DIM) const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] + rhs.elements[i];

	return result;
}

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator-(LUCID_VECTOR(T, DIM) const &lhs, LUCID_VECTOR(T, DIM) const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] - rhs.elements[i];

	return result;
}

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator*(LUCID_VECTOR(T, DIM) const &lhs, T const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

	result.elements[0] = lhs.elements[0] * rhs;
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] * rhs;

	return result;
}

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator*(T const &lhs, LUCID_VECTOR(T, DIM) const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

	result.elements[0] = lhs * rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs * rhs.elements[i];

	return result;
}

template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) operator/(LUCID_VECTOR(T, DIM) const &lhs, T const &rhs)
{
	LUCID_VECTOR(T, DIM) result;

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
template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) lerp(T const &t, LUCID_VECTOR(T, DIM) const &a, LUCID_VECTOR(T, DIM) const &b)
{
	return LUCID_VECTOR(T, DIM)((b - a) * t + a);
}

///	dot
///
///	dot product
template<typename T, size_t DIM> inline T dot(LUCID_VECTOR(T, DIM) const &lhs, LUCID_VECTOR(T, DIM) const &rhs)
{
	T result = lhs.elements[0] * rhs.elements[0];

	for (size_t i = 1; i < DIM; ++i)
		result = result + lhs.elements[i] * rhs.elements[i];

	return result;
}

///	lsq
///
///	length squared
template<typename T, size_t DIM> inline T lsq(LUCID_VECTOR(T, DIM) const &rhs)
{
	return dot(rhs, rhs);
}

///	len
///
///	length
template<typename T, size_t DIM> inline T len(LUCID_VECTOR(T, DIM) const &rhs)
{
	return std::sqrt(dot(rhs, rhs));
}

///	normalize
///
///	normalize vector
template<typename T, size_t DIM> inline LUCID_VECTOR(T, DIM) normalize(LUCID_VECTOR(T, DIM) const &rhs)
{
	return rhs / len(rhs);
}

///	cross
///
///	3D vector cross product
template<typename T> inline LUCID_VECTOR(T, 3) cross(LUCID_VECTOR(T, 3) const &lhs, LUCID_VECTOR(T, 3) const &rhs)
{
	LUCID_VECTOR(T, 3) result;

	result.x = lhs.y * rhs.z - rhs.y * lhs.z;
	result.y = rhs.x * lhs.z - lhs.x * rhs.z;
	result.z = lhs.x * rhs.y - rhs.x * lhs.y;
		
	return result;
}

LUCID_MATH_END
