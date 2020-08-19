#pragma once

#include <cmath>
#include <lucid/units/Quantity.h>
#include <lucid/units/Meta.h>
#include <lucid/units/System.h>

///
///
///

namespace lucid {
namespace units {

	///	Measure (noun)
	///
	///	an actual measurement of some quantity using a particular system
	template<typename T, typename U = Units<system::none, quantity::pure> > struct Measure
	{
		typedef typename T value_t;
		typedef typename U::system_t system_t;
		typedef typename U::quantity_t quantity_t;
		typedef typename U units_t;
		typedef typename Measure<T,U> self_t;

		value_t value = value_t();

		Measure() = default;

		Measure(value_t const &rhs)
			: value(rhs)
		{
		}

		~Measure() = default;

		self_t &operator=(value_t const &rhs)
		{
			value = rhs;
			return *this;
		}
	};

}	///	units
}	///	lucid

#pragma push_macro("MEASURE")
#define MEASURE(T,S,Q) ::lucid::units::Measure<T,::lucid::units::Units<S,Q> >

#pragma push_macro("ADD")
#define ADD(LHS,RHS) typename ::lucid::units::qty_add<LHS,RHS>::result

#pragma push_macro("SUB")
#define SUB(LHS,RHS) typename ::lucid::units::qty_sub<LHS,RHS>::result

#pragma push_macro("MUL")
#define MUL(LHS,RHS) typename ::lucid::units::qty_mul<LHS,RHS>::result

#pragma push_macro("DIV")
#define DIV(LHS,RHS) typename ::lucid::units::qty_div<LHS,RHS>::result

template<typename T, typename S, typename Q> inline MEASURE(T,S,Q) operator-(MEASURE(T,S,Q) const &rhs)
{
	MEASURE(T,S,Q) lval;
	lval.value = -rhs.value;
	return lval;
}

template<typename T, typename S, typename Q> inline MEASURE(T,S,Q) operator+(MEASURE(T,S,Q) const &lhs, MEASURE(T,S,Q) const &rhs)
{
	MEASURE(T,S,Q) lval;
	lval.value = lhs.value + rhs.value;
	return lval;
}

template<typename T, typename S, typename Q> inline MEASURE(T,S,Q) operator-(MEASURE(T,S,Q) const &lhs, MEASURE(T,S,Q) const &rhs)
{
	MEASURE(T,S,Q) lval;
	lval.value = lhs.value - rhs.value;
	return lval;
}

template<typename T, typename S, typename LQ, typename RQ> inline MEASURE(T,S,ADD(LQ,RQ)) operator*(MEASURE(T,S,LQ) const &lhs, MEASURE(T,S,RQ) const &rhs)
{
	MEASURE(T,S,ADD(LQ,RQ)) lval;
	lval.value = lhs.value * rhs.value;
	return lval;
}

template<typename T, typename S, typename LQ, typename RQ> inline MEASURE(T,S,ADD(LQ,RQ)) operator/(MEASURE(T,S,LQ) const &lhs, MEASURE(T,S,RQ) const &rhs)
{
	MEASURE(T,S,SUB(LQ,RQ)) lval;
	lval.value = lhs.value / rhs.value;
	return lval;
}

///
///
///

namespace lucid {
namespace units {

	template<typename S, typename Q, typename T> inline MEASURE(T,S,Q) abs(MEASURE(T,S,Q) const &rhs)
	{
		return MEASURE(T,S,Q)(std::abs(rhs.value));
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
