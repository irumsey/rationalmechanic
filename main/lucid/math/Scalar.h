#pragma once

#include <lucid/units/System.h>
#include <lucid/math/Defines.h>

LUCID_MATH_BEGIN

template<typename T, typename Q> struct Scalar
{
	T value = T();

	Scalar() = default;

	~Scalar() = default;

	constexpr Scalar(T const &rhs)
		: value(rhs)
	{
	}
};

LUCID_MATH_END

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T,S,Q) operator-(LUCID_SCALAR(T,S,Q) const &rhs)
{
	return LUCID_SCALAR(T,S,Q)(-rhs.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T,S,Q) operator+(LUCID_SCALAR(T,S,Q) const &lhs, LUCID_SCALAR(T,S,Q) const &rhs)
{
	return LUCID_SCALAR(T,S,Q)(lhs.value + rhs.value);
}

template<typename T, typename S, typename Q>
inline LUCID_SCALAR(T,S,Q) operator-(LUCID_SCALAR(T,S,Q) const &lhs, LUCID_SCALAR(T,S,Q) const &rhs)
{
	return LUCID_SCALAR(T,S,Q)(lhs.value - rhs.value);
}

template<typename T, typename S, typename LHQ, typename RHQ>
inline LUCID_SCALAR(T,S,LUCID_QUANTITY_ADD(LHQ,RHQ)) operator*(LUCID_SCALAR(T,S,LHQ) const &lhs, LUCID_SCALAR(T,S,RHQ) const &rhs)
{
	return LUCID_SCALAR(T,S,LUCID_QUANTITY_ADD(LHQ,RHQ))(lhs.value * rhs.value);
}

template<typename T, typename S, typename LHQ, typename RHQ>
inline LUCID_SCALAR(T,S,LUCID_QUANTITY_SUB(LHQ,RHQ)) operator/(LUCID_SCALAR(T,S,LHQ) const &lhs, LUCID_SCALAR(T,S,RHQ) const &rhs)
{
	return LUCID_SCALAR(T,S,LUCID_QUANTITY_SUB(LHQ,RHQ))(lhs.value / rhs.value);
}
