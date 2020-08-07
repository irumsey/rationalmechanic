#pragma once

#include <cinttypes>
#include <cmath>
#include <string>
#include <lucid/core/Types.h>
#include <lucid/math/Integer.h>

///
///
///
namespace lucid {
namespace math {

	///	Fixed
	///
	///	Used in conjunction with math::Integer
	template<uint16_t B, uint16_t D> struct Fixed
	{
		enum {   TOTAL_BITS = B };
		enum {   WHOLE_BITS = B - D };
		enum { DECIMAL_BITS = D };

		typedef Integer<B> integer_t;
		typedef Fixed<B,D> self_t;

		integer_t raw;

		Fixed() = default;

		Fixed(float32_t rhs)
		{
			_set(*this, rhs);
		}

		Fixed(std::string const &rhs)
		{
			_set(*this, rhs);
		}

		~Fixed() = default;

		Fixed &operator=(float32_t rhs)
		{
			_set(*this, rhs);
			return *this;
		}

		Fixed &operator=(std::string const &rhs)
		{
			_set(*this, rhs);
			return *this;
		}

		/// TBD: figure out a clever way of making this a constant
		static integer_t blah()
		{
			integer_t const ten(10);
			integer_t value(1);
			for (size_t i = 0; i < DECIMAL_BITS; ++i)
				integer_t::_mul(value, value, ten);
			return value;
		}

		static void _set(self_t &lval, std::string const &rhs)
		{
			lval.raw = integer_t();

			int32_t const count = rhs.size();
			int32_t const    dp = rhs.find_first_of('.');
			int32_t const  last = count - 1;

			if (0 == count)
				return;

			bool isNegative = ('-' == rhs[0]);
			int32_t const end = isNegative ? 0 : -1;

			integer_t magnitude(1);
			integer_t const ten(10);

			integer_t scale(1);
			integer_t value;
			for (int32_t i = last; i > end; --i)
			{
				if (',' == rhs[i])
					continue;

				if ('.' != rhs[i])
				{
					uint16_t digit = rhs[i] - '0';
					LUCID_VALIDATE((0 <= digit) && (digit <= 9), "invalid character in string");

					integer_t::_mul(value, integer_t(digit), magnitude);
					integer_t::_add(lval.raw, lval.raw, value);
					integer_t::_mul(magnitude, magnitude, ten);
				}
				else
				{
					scale = magnitude;
				}
			}

			integer_t::_lsh(lval.raw, lval.raw, DECIMAL_BITS);
			integer_t::_div(lval.raw, lval.raw,        scale);

			if (isNegative)
				_neg(lval, lval);
		}

		///	utility for setting "small" values
		static void _set(self_t &lval, float32_t rhs)
		{
			uint32_t value = uint32_t(rhs * (1 << DECIMAL_BITS));
			lval.raw.data[0] = (0x0000FFFF & value);
			lval.raw.data[1] = (0xFFFF0000 & value) >> integer_t::SHIFT;
		}

		static void _repr(std::string &lval, self_t const &rhs)
		{
			integer_t whole;
			integer_t::_rsh (whole, rhs.raw, DECIMAL_BITS);
			integer_t::_repr( lval,   whole);

			lval.push_back('.');

			integer_t frac;
			integer_t::_lsh(whole,   whole,  DECIMAL_BITS);
			integer_t::_sub( frac, rhs.raw,         whole);

			integer_t::_mul(frac, frac, blah());
			integer_t::_rsh(frac, frac, DECIMAL_BITS);

			std::string decimal;
			integer_t::_repr( decimal,   frac);

			decimal = (0 == decimal.size()) ? "0" : decimal;

			lval = lval + decimal;
		}

		static void _neg(self_t &lval, self_t const &rhs)
		{
			integer_t::_neg(lval.raw, rhs.raw);
		}

		static void _add(self_t &lval, self_t const &lhs, self_t const &rhs)
		{
			integer_t::_add(lval.raw, lhs.raw, rhs.raw);
		}

		static void _sub(self_t &lval, self_t const &lhs, self_t const &rhs)
		{
			integer_t::_sub(lval.raw, lhs.raw, rhs.raw);
		}

		static void _mul(self_t &lval, self_t const &lhs, self_t const &rhs)
		{
			integer_t::_mul(lval.raw, lhs.raw, rhs.raw);
			integer_t::_rsh(lval.raw, lval.raw, DECIMAL_BITS);
		}

		static void _div(self_t &lval, self_t const &lhs ,self_t const &rhs)
		{
			integer_t::_div(lval.raw, lhs.raw, rhs.raw);
			integer_t::_lsh(lval.raw, lval.raw, DECIMAL_BITS);
		}

	};

	///
	///	expose equal and not equal methods for symmetry (matrix, vector, quaternion, scalar, etc.. all define these)
	///

	template<size_t N, size_t D> inline bool equ(Fixed<N,D> const lhs, Fixed<N,D> const rhs)
	{
		return Fixed<N,D>::integer_t::_equ(lhs.raw, rhs.raw);
	}

	template<size_t N, size_t D> inline bool neq(Fixed<N,D> const lhs, Fixed<N,D> const rhs)
	{
		return Fixed<N,D>::integer_t::_neq(lhs.raw, rhs.raw);
	}

}	/// math
}	/// lucid

#pragma push_macro("NUMBER")
#define NUMBER(B,D) ::lucid::math::Fixed<B,D>

template<size_t B, size_t D> inline NUMBER(B,D) operator-(NUMBER(B,D) const &rhs)
{
	NUMBER(B,D) result;
	NUMBER(B,D)::_neg(result, rhs);
	return result;
}

template<size_t B, size_t D> inline NUMBER(B,D) operator+(NUMBER(B,D) const &lhs, NUMBER(B,D) const &rhs)
{
	NUMBER(B,D) result;
	NUMBER(B,D)::_add(result, lhs, rhs);
	return result;
}

template<size_t B, size_t D> inline NUMBER(B,D) operator-(NUMBER(B,D) const &lhs, NUMBER(B,D) const &rhs)
{
	NUMBER(B,D) result;
	NUMBER(B,D)::_sub(result, lhs, rhs);
	return result;
}

template<size_t B, size_t D> inline NUMBER(B,D) operator*(NUMBER(B,D) const &lhs, NUMBER(B,D) const &rhs)
{
	NUMBER(B,D) result;
	NUMBER(B,D)::_mul(result, lhs, rhs);
	return result;
}

template<size_t B, size_t D> inline NUMBER(B,D) operator/(NUMBER(B,D) const &lhs, NUMBER(B,D) const &rhs)
{
	NUMBER(B,D) result;
	NUMBER(B,D)::_div(result, lhs, rhs);
	return result;
}

#undef NUMBER
#pragma pop_macro("NUMBER")
