#pragma once

#include <cassert>
#include <lucid/units/System.h>
#include <lucid/math/Scalar.h>

///
///	
///

#pragma push_macro("QUANTITY_PURE")
#define QUANTITY_PURE ::lucid::units::quantity::pure

#pragma push_macro("SCALAR")
#define SCALAR(T, S, Q) ::lucid::math::Scalar<T, ::lucid::units::Unit<S, Q> >

#pragma push_macro("VECTOR")
#define VECTOR(T, DIM, S, Q) ::lucid::math::Vector<T, DIM, ::lucid::units::Unit<S, Q> >

#pragma push_macro("ADD")
#define ADD(LHS, RHS) typename ::lucid::units::quantity::add<LHS, RHS>::result

#pragma push_macro("SUB")
#define SUB(LHS, RHS) typename ::lucid::units::quantity::sub<LHS, RHS>::result

#pragma push_macro("HALF")
#define HALF(Q) typename ::lucid::units::quantity::half<Q>::result

///
///
///

namespace lucid {
namespace math {

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

}	///	math
}	///	lucid

///
///	
///

template<typename T, size_t DIM, typename S, typename Q>
inline VECTOR(T, DIM, S, Q) operator-(VECTOR(T, DIM, S, Q) const &rhs)
{
	VECTOR(T, DIM, S, Q) result;

	result.elements[0] = -rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = -rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename Q>
inline VECTOR(T, DIM, S, Q) operator+(VECTOR(T, DIM, S, Q) const &lhs, VECTOR(T, DIM, S, Q) const &rhs)
{
	VECTOR(T, DIM, S, Q) result;

	result.elements[0] = lhs.elements[0] + rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] + rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename Q>
inline VECTOR(T, DIM, typename S, typename Q) operator-(VECTOR(T, DIM, S, Q) const &lhs, VECTOR(T, DIM, S, Q) const &rhs)
{
	VECTOR(T, DIM, S, Q) result;

	result.elements[0] = lhs.elements[0] - rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] - rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline VECTOR(T, DIM, S, ADD(LQ, RQ)) operator*(VECTOR(T, DIM, S, LQ) const &lhs, SCALAR(T, S, RQ) const &rhs)
{
	VECTOR(T, DIM, S, ADD(LQ, RQ)) result;

	result.elements[0] = lhs.elements[0] * rhs.value;
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.elements[i] * rhs.value;

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline VECTOR(T, DIM, S, ADD(LQ, RQ)) operator*(SCALAR(T, S, LQ) const &lhs, VECTOR(T, DIM, S, RQ) const &rhs)
{
	VECTOR(T, DIM, S, ADD(LQ, RQ)) result;

	result.elements[0] = lhs.value * rhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = lhs.value * rhs.elements[i];

	return result;
}

template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
inline VECTOR(T, DIM, S, SUB(LQ, RQ)) operator/(VECTOR(T, DIM, S, LQ) const &lhs, SCALAR(T, S, RQ) const &rhs)
{
	VECTOR(T, DIM, S, SUB(LQ, RQ)) result;

	T const coeff = T(1) / rhs.value;

	result.elements[0] = coeff * lhs.elements[0];
	for (size_t i = 1; i < DIM; ++i)
		result.elements[i] = coeff * lhs.elements[i];

	return result;
}

///
///
///

namespace lucid {
namespace math {

	///	interpolate
	///
	///	linear interpolation from a to b.
	template<typename T, size_t DIM, typename S, typename Q>
	inline VECTOR(T, DIM, S, Q) lerp(SCALAR(T, S, QUANTITY_PURE) const &t, VECTOR(T, DIM, S, Q) const &a, VECTOR(T, DIM, S, Q) const &b)
	{
		return VECTOR(T, DIM, S, Q)((b - a) * t + a);
	}

	///	dot
	///
	///	dot product
	template<typename T, size_t DIM, typename S, typename LQ, typename RQ>
	inline SCALAR(T, S, ADD(LQ, RQ)) dot(VECTOR(T, DIM, S, LQ) const &lhs, VECTOR(T, DIM, S, RQ) const &rhs)
	{
		SCALAR(T, S, ADD(LQ, RQ)) result = lhs.elements[0] * rhs.elements[0];

		for (size_t i = 1; i < DIM; ++i)
			result.value += lhs.elements[i] * rhs.elements[i];

		return result;
	}

	///	lsq
	///
	///	length squared
	template<typename T, size_t DIM, typename S, typename Q>
	inline SCALAR(T, S, ADD(Q, Q)) lsq(VECTOR(T, DIM, S, Q) const &rhs)
	{
		return dot(rhs, rhs);
	}

	///	len
	///
	///	length
	template<typename T, size_t DIM, typename S, typename Q>
	inline SCALAR(T, S, Q) len(VECTOR(T, DIM, S, Q) const &rhs)
	{
		return SCALAR(T, S, Q)(std::sqrt(dot(rhs, rhs).value));
	}

	///	normalize
	///
	///	normalize vector
	template<typename T, size_t DIM, typename S, typename Q>
	inline VECTOR(T, DIM, S, QUANTITY_PURE) normalize(VECTOR(T, DIM, S, Q) const &rhs)
	{
		return rhs / len(rhs);
	}

	///	cross
	///
	///	3D vector cross product
	template<typename T, typename S, typename Q>
	inline VECTOR(T, 3, S, ADD(Q, Q)) cross(VECTOR(T, 3, S, Q) const &lhs, VECTOR(T, 3, S, Q) const &rhs)
	{
		VECTOR(T, 3, S, ADD(Q, Q)) result;

		result.x = lhs.y * rhs.z - rhs.y * lhs.z;
		result.y = rhs.x * lhs.z - lhs.x * rhs.z;
		result.z = lhs.x * rhs.y - rhs.x * lhs.y;
		
		return result;
	}

}	/// math
}	///	lucid

///
///	remove the shorthands
///

#undef HALF
#pragma pop_macro("HALF")

#undef SUB
#pragma pop_macro("SUB")

#undef ADD
#pragma pop_macro("ADD")

#undef VECTOR
#pragma pop_macro("VECTOR")

#undef SCALAR
#pragma pop_macro("SCALAR")

#undef QUANTITY_PURE
#pragma pop_macro("QUANTITY_PURE")