#pragma once

#include <algorithm>
#include <cmath>
#include <lucid/units/System.h>

#ifdef min
#	undef min
#	undef max
#endif

///
///
///

#pragma push_macro("QUANTITY_PURE")
#define QUANTITY_PURE ::lucid::units::quantity::pure

#pragma push_macro("SCALAR")
#define SCALAR(T, S, Q) ::lucid::math::Scalar<T, ::lucid::units::Unit<S, Q> >

#pragma push_macro("ADD")
#define ADD(LHS, RHS) typename ::lucid::units::quantity::add<LHS, RHS>::result

#pragma push_macro("SUB")
#define SUB(LHS, RHS) typename ::lucid::units::quantity::sub<LHS, RHS>::result

#pragma push_macro("HALF")
#define HALF(RHS) typename ::lucid::units::quantity::half<RHS>::result

#pragma push_macro("NEG")
#define NEG(RHS) typename ::lucid::units::quantity::neg<RHS>::result

///
///
///

namespace lucid {
namespace math {

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

}	///	math
}	///	lucid

///
///
///

template<typename T, typename S, typename Q>
inline bool operator<(SCALAR(T, S, Q) const &lhs, SCALAR(T, S, Q) const &rhs)
{
	return lhs.value < rhs.value;
}

template<typename T, typename S, typename Q>
inline bool operator>(SCALAR(T, S, Q) const &lhs, SCALAR(T, S, Q) const &rhs)
{
	return lhs.value > rhs.value;
}

template<typename T, typename S, typename Q>
inline SCALAR(T, S, Q) operator-(SCALAR(T, S, Q) const &rhs)
{
	return SCALAR(T, S, Q)(-rhs.value);
}

template<typename T, typename S, typename Q>
inline SCALAR(T, S, Q) operator+(SCALAR(T, S, Q) const &lhs, SCALAR(T, S, Q) const &rhs)
{
	return SCALAR(T, S, Q)(lhs.value + rhs.value);
}

template<typename T, typename S, typename Q>
inline SCALAR(T, S, Q) operator-(SCALAR(T, S, Q) const &lhs, SCALAR(T, S, Q) const &rhs)
{
	return SCALAR(T, S, Q)(lhs.value - rhs.value);
}

template<typename T, typename S, typename LQ, typename RQ>
inline SCALAR(T, S, ADD(LQ, RQ)) operator*(SCALAR(T, S, LQ) const &lhs, SCALAR(T, S, RQ) const &rhs)
{
	return SCALAR(T, S, ADD(LQ, RQ))(lhs.value * rhs.value);
}

template<typename T, typename S, typename LQ, typename RQ>
inline SCALAR(T, S, SUB(LQ, RQ)) operator/(SCALAR(T, S, LQ) const &lhs, SCALAR(T, S, RQ) const &rhs)
{
	return SCALAR(T, S, SUB(LQ, RQ))(lhs.value / rhs.value);
}

///
///
///

namespace lucid {
namespace math {

	/// min / max
	/// 
	/// 

	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) min(SCALAR(T, S, Q) const &x, SCALAR(T, S, Q) const &y)
	{
		return SCALAR(T, S, Q)((x.value <= y.value) ? x.value : y.value);
	}

	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) max(SCALAR(T, S, Q) const &x, SCALAR(T, S, Q) const &y)
	{
		return SCALAR(T, S, Q)((x.value >= y.value) ? x.value : y.value);
	}

	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) abs(SCALAR(T, S, Q) const &rhs)
	{
		return std::abs(rhs.value);
	}

	///	interpolate
	///
	///	linear interpolation from a to b.
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) lerp(SCALAR(T, S, QUANTITY_PURE) const &t, SCALAR(T, S, Q) const &a, SCALAR(T, S, Q) const &b)
	{
		return SCALAR(T, S, Q)((b.value - a.value) * t.value + a.value);
	}

	///	clamp
	///
	///	clamp value between values a and b
	///	where a < b
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) clamp(SCALAR(T, S, Q) const &x, SCALAR(T, S, Q) const &a, SCALAR(T, S, Q) const &b)
	{
		return SCALAR(T, S, Q)(std::min(std::max(a.value, x.value), b.value));
	}

	///	exclude
	///
	///	range of exclusion for the value x defined by a and b
	///	where a < b
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, Q) exclude(SCALAR(T, S, Q) const &x, SCALAR(T, S, Q) const &a, SCALAR(T, S, Q) const &b)
	{
		if ((x.value < a.value) || (b.value < x.value))
			return x;

		if ((b.value - x.value) > (x.value - a.value))
			return a;

		return SCALAR(T, S, Q)(b.value);
	}

	///
	///
	///
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, NEG(Q)) inv(SCALAR(T, S, Q) const &rhs)
	{
		return SCALAR(T, S, NEG(Q))(T(1) / rhs.value);
	}

	///
	///
	///
	template<typename T, typename S, typename Q>
	inline SCALAR(T, S, HALF(Q)) sqrt(SCALAR(T, S, Q) const &rhs)
	{
		return SCALAR(T, S, HALF(Q))(std::sqrt(rhs.value));
	}

	///
	///	cosine "wrappers"
	///

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) cos(SCALAR(T, S, QUANTITY_PURE) const &theta)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::cos(theta.value));
	}

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) acos(SCALAR(T, S, QUANTITY_PURE) const &x)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::acos(x.value));
	}

	///
	///	sine "wrappers"
	///

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) sin(SCALAR(T, S, QUANTITY_PURE) const &theta)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::sin(theta.value));
	}

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) asin(SCALAR(T, S, QUANTITY_PURE) const &x)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::asin(x.value));
	}

	///
	///	tangent "wrappers"
	///

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) tan(SCALAR(T, S, QUANTITY_PURE) const &theta)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::tan(theta.value));
	}

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) atan(SCALAR(T, S, QUANTITY_PURE) const &x)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::atan(x.value));
	}

	template<typename T, typename S>
	inline SCALAR(T, S, QUANTITY_PURE) atan2(SCALAR(T, S, QUANTITY_PURE) const &y, SCALAR(T, S, QUANTITY_PURE) const &x)
	{
		return SCALAR(T, S, QUANTITY_PURE)(std::atan2(y.value, x.value));
	}

}	///	math
}	///	lucid

#undef NEG
#pragma pop_macro("NEG")

#undef HALF
#pragma pop_macro("HALF")

#undef SUB
#pragma pop_macro("SUB")

#undef ADD
#pragma pop_macro("ADD")

#undef SCALAR
#pragma pop_macro("SCALAR")

#undef QUANTITY_PURE
#pragma pop_macro("QUANTITY_PURE")