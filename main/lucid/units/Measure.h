#pragma once

#include <cmath>
#include <lucid/units/Quantity.h>
#include <lucid/units/Meta.h>

namespace lucid {
namespace units {

	///	Measure (noun)
	///
	///	an actual measurement of some quantity using a particular system
	template<typename S, typename Q, typename T> struct Measure
	{
		typedef typename Measure<S,Q,T> self_t;

		T value = T();

		Measure() = default;

		Measure(T const &rhs)
			: value(rhs)
		{
		}

		~Measure() = default;

		self_t &operator=(T const &rhs)
		{
			value = rhs;
			return *this;
		}
	};

}	///	units
}	///	lucid

#pragma push_macro("MEASURE")
#define MEASURE(S,Q,T) ::lucid::units::Measure<S,Q,T>

#pragma push_macro("ADD")
#define ADD(LHS,RHS) typename ::lucid::units::add<LHS,RHS>::result

#pragma push_macro("SUB")
#define SUB(LHS,RHS) typename ::lucid::units::sub<LHS,RHS>::result

#pragma push_macro("MUL")
#define MUL(LHS,RHS) typename ::lucid::units::mul<LHS,RHS>::result

#pragma push_macro("DIV")
#define DIV(LHS,RHS) typename ::lucid::units::div<LHS,RHS>::result

template<typename S, typename Q, typename T> inline MEASURE(S,Q,T) operator-(MEASURE(S,Q,T) const &rhs)
{
	MEASURE(S,Q,T) lval;
	lval.value = -rhs.value;
	return lval;
}

template<typename S, typename Q, typename T> inline MEASURE(S,Q,T) operator+(MEASURE(S,Q,T) const &lhs, MEASURE(S,Q,T) const &rhs)
{
	MEASURE(S,Q,T) lval;
	lval.value = lhs.value + rhs.value;
	return lval;
}

template<typename S, typename Q, typename T> inline MEASURE(S,Q,T) operator-(MEASURE(S,Q,T) const &lhs, MEASURE(S,Q,T) const &rhs)
{
	MEASURE(S,Q,T) lval;
	lval.value = lhs.value - rhs.value;
	return lval;
}

template<typename S, typename LHS, typename RHS, typename T> inline MEASURE(S,ADD(LHS,RHS),T) operator*(MEASURE(S,LHS,T) const &lhs, MEASURE(S,RHS,T) const &rhs)
{
	MEASURE(S,ADD(LHS,RHS),T) lval;
	lval.value = lhs.value * rhs.value;
	return lval;
}

template<typename S, typename LHS, typename RHS, typename T> inline MEASURE(S,SUB(LHS,RHS),T) operator/(MEASURE(S,LHS,T) const &lhs, MEASURE(S,RHS,T) const &rhs)
{
	MEASURE(S,SUB(LHS,RHS),T) lval;
	lval.value = lhs.value / rhs.value;
	return lval;
}

///
///
///

namespace lucid {
namespace units {

	template<typename S, typename Q, typename T> inline MEASURE(S,Q,T) abs(MEASURE(S,Q,T) const &rhs)
	{
		return MEASURE(S,Q,T)(std::abs(rhs.value));
	}

}	///	units
}	///	lucid

#undef DIV
#pragma pop_macro("DIV")

#undef MUL
#pragma pop_macro("MUL")

#undef SUB
#pragma pop_macro("SUB")

#undef ADD
#pragma pop_macro("ADD")

#undef MEASURE
#pragma pop_macro("MEASURE")
