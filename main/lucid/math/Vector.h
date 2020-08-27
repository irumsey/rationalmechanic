#pragma once

#include <cassert>
#include <lucid/math/Constants.h>

///
///
///
namespace lucid {
namespace math {

	///	Vector<T,DIM>
	///
	///	general vector template
	template<typename T, int DIM> struct Vector
	{
		T elements[DIM];

		Vector() = default;

		~Vector() = default;

		T &operator[](int32_t i)
		{
			assert(i < DIM);
			return elements[i];
		}

		T const &operator[](int32_t i) const
		{
			assert(i < DIM);
			return elements[i];
		}
	};

	///	Vector<T,2>
	///
	///	2D specialization of Vector<>
	///	providing (x,y) members.
	template<typename T> struct Vector<T,2>
	{
		union
		{
			T elements[2];
			struct { T x, y; };
		};

		Vector() = default;

		~Vector() = default;

		Vector(T const &x, T const &y)
			: x(x), y(y)
		{
		}

		T &operator[](int32_t i)
		{
			assert(i < 2);
			return elements[i];
		}

		T const &operator[](int32_t i) const
		{
			assert(i < 2);
			return elements[i];
		}
	};

	///	Vector<T,3>
	///
	///	3D specialization of Vector<>
	///	providing (x,y,z) members.
	template<typename T> struct Vector<T,3>
	{
		union
		{
			T elements[3];
			struct { T x, y, z; };
		};

		Vector() = default;

		~Vector() = default;

		Vector(T const &x, T const &y, T const &z)
			: x(x), y(y), z(z)
		{
		}

		T &operator[](int32_t i)
		{
			assert(i < 3);
			return elements[i];
		}

		T const &operator[](int32_t i) const
		{
			assert(i < 3);
			return elements[i];
		}
	};

	///	Vector<T,4>
	///
	///	4D specialization of Vector<>
	///	providing (x,y,z,w) members.
	template<typename T> struct Vector<T,4>
	{
		union
		{
			T elements[4];
			struct { T x, y, z, w; };
		};

		Vector() = default;

		~Vector() = default;

		Vector(T const &x, T const &y, T const &z, T const &w)
			: x(x), y(y), z(z), w(w)
		{
		}

		T &operator[](int32_t i)
		{
			assert(i < 4);
			return elements[i];
		}

		T const &operator[](int32_t i) const
		{
			assert(i < 4);
			return elements[i];
		}
	};

}	///	math
}	///	lucid

///	vector shorthand for the following operator overloads
///
///	note: relies upon push/pop macro pragmas
#pragma push_macro("VECTOR")
#define VECTOR(T,DIM) ::lucid::math::Vector<T,DIM>

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator-(VECTOR(T,DIM) const &rhs)
{
	VECTOR(T,DIM) result;

	result.elements[0] = -rhs.elements[0];
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = -rhs.elements[i];
	}

	return result;
}

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator+(VECTOR(T,DIM) const &lhs, VECTOR(T,DIM) const &rhs)
{
	VECTOR(T,DIM) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = lhs.elements[i] + rhs.elements[i];
	}

	return result;
}

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator-(VECTOR(T,DIM) const &lhs, VECTOR(T,DIM) const &rhs)
{
	VECTOR(T,DIM) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = lhs.elements[i] - rhs.elements[i];
	}

	return result;
}

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator*(VECTOR(T,DIM) const &lhs, T const &rhs)
{
	VECTOR(T,DIM) result;

	result.elements[0] = lhs.elements[0] * rhs;
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = lhs.elements[i] * rhs;
	}

	return result;
}

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator*(T const &lhs, VECTOR(T,DIM) const &rhs)
{
	VECTOR(T,DIM) result;

	result.elements[0] = lhs * rhs.elements[0];
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = lhs * rhs.elements[i];
	}

	return result;
}

///
///
///
template<typename T, int DIM> inline VECTOR(T,DIM) operator/(VECTOR(T,DIM) const &lhs, T const &rhs)
{
	VECTOR(T,DIM) result;

	T const coeff = ::lucid::math::constants::one<T>() / rhs;

	result.elements[0] = coeff * lhs.elements[0];
	for (int32_t i = 1; i < DIM; ++i)
	{
		result.elements[i] = coeff * lhs.elements[i];
	}

	return result;
}

///
///	remove the vector shorthand
///
#undef VECTOR
#pragma pop_macro("VECTOR")

///
///
///
namespace lucid {
namespace math {

	///	dot
	///
	///	dot product
	template<typename T, int DIM> inline T dot(Vector<T,DIM> const &lhs, Vector<T,DIM> const &rhs)
	{
		T result = lhs.elements[0] * rhs.elements[0];

		for (int32_t i = 1; i < DIM; ++i)
		{
			result += lhs.elements[i] * rhs.elements[i];
		}

		return result;
	}

	///	lsq
	///
	///	length squared
	template<typename T, int DIM> inline T lsq(Vector<T,DIM> const &rhs)
	{
		return dot(rhs, rhs);
	}

	///	len
	///
	///	length
	template<typename T, int DIM> inline T len(Vector<T,DIM> const &rhs)
	{
		return sqrt(dot(rhs, rhs));
	}

	///	normalize
	///
	///	normalize vector
	template<typename T, int DIM> inline Vector<T,DIM> normalize(Vector<T,DIM> const &rhs)
	{
		return rhs / len(rhs);
	}

	///	cross
	///
	///	3D vector cross product
	template<typename T> inline Vector<T,3> cross(Vector<T,3> const &lhs, Vector<T,3> const &rhs)
	{
		return Vector<T,3>
		(
			lhs.y * rhs.z - rhs.y * lhs.z,
			rhs.x * lhs.z - lhs.x * rhs.z,
			lhs.x * rhs.y - rhs.x * lhs.y
		);
	}

	///	equ and neq
	///
	///	equality tests

	template<typename T, int DIM> inline bool equ(Vector<T,DIM> const &lhs, Vector<T,DIM> const &rhs)
	{
		return lsq(lhs - rhs) < constants::tol_tol<T>();
	}

	template<typename T, int DIM> inline bool neq(Vector<T,DIM> const &lhs, Vector<T,DIM> const &rhs)
	{
		return !equ(lhs, rhs);
	}

}	/// math
}	///	lucid
