#pragma once

#include <cassert>
#include <cinttypes>
#include <algorithm>
#include <string>
#include <lucid/core/Error.h>
#include <lucid/core/Array.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Algorithm.h>

LUCID_MATH_BEGIN

///
///
///  Note: the number of bits, N, must be a multiple of 16.
///	 this uses uint16_t as a "digit", therefore, can be treated
///	 as a base 65536 integer.
template<size_t N> struct Integer
{
	enum {  BITS = N };
	enum { COUNT = N >> 4 };
	enum {  LAST = COUNT - 1 };
	enum {  MASK = 0x0000FFFF };
	enum { SHIFT = 16 };

	typedef typename Integer<N> self_t;
	typedef typename LUCID_CORE::Array<COUNT, uint16_t> data_t;

	data_t data;

	Integer() = default;

	Integer(std::string const &rhs)
	{
		_set(*this, rhs);
	}

	Integer(uint16_t rhs)
	{
		_set(*this, rhs);
	}

	~Integer() = default;

	Integer &operator=(std::string const &rhs)
	{
		_set(*this, rhs);
		return *this;
	}

	Integer &operator=(uint16_t rhs)
	{
		_set(*this, rhs);
		return *this;
	}

	static void _set(self_t &lval, std::string const &rhs)
	{
		lval.data = data_t();

		int32_t const count = int32_t(rhs.size());
		int32_t const last = int32_t(count - 1);
		if (0 == count)
			return;

		bool isNegative = ('-' == rhs[0]);
		int32_t const end = isNegative ? 0 : -1;

		self_t magnitude(1);
		self_t const ten(10);

		self_t value;
		for (int32_t i = last; i > end; --i)
		{
			if (',' == rhs[i])
				continue;

			uint16_t digit = rhs[i] - '0';
			LUCID_VALIDATE((0 <= digit) && (digit <= 9), "invalid character in string");

			_mul(value, self_t(digit), magnitude);
			_add(lval, lval, value);
			_mul(magnitude, magnitude, ten);
		}

		if (isNegative)
			_neg(lval, lval);
	}

	/// just for symmetry
	static void _set(self_t &lval, self_t const &rhs)
	{
		lval = rhs;
	}

	/// utility for setting "small" values
	static void _set(self_t &lval, uint16_t rhs)
	{
		lval.data = data_t();
		lval.data[0] = rhs;
	}

	static void _repr(std::string &lval, self_t const &rhs)
	{
		static char const c[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', };

		bool isNegative = _ltz(rhs);
		self_t tmp = rhs;

		if (isNegative)
			_abs(tmp, tmp);

		lval.clear();

		self_t const zero( 0);
		self_t const  ten(10);

		while (_gt(tmp, zero))
		{
			self_t Q;
			self_t R;

			_div_mod(R, Q, tmp, ten);
			LUCID_VALIDATE((0 <= R.data[0]) && (R.data[0] <= 9), "error creating repr of large integer");

			lval.push_back(c[R.data[0]]);
			tmp = Q;
		}

		if (isNegative)
			lval.push_back('-');

		std::reverse(lval.begin(), lval.end());
	}

	static bool _equ(self_t const &lhs, self_t const &rhs)
	{
		for (size_t i = 0; i < COUNT; ++i)
			if (lhs.data[i] != rhs.data[i])
				return false;
		return true;
	}

	static bool _neq(self_t const &lhs, self_t const &rhs)
	{
		return !_equ(lhs, rhs);
	}

	static bool _ltz(self_t const &rhs)
	{
		return (0 != (0x8000 & rhs.data[LAST]));
	}

	static bool _lt(self_t const &lhs, self_t const &rhs)
	{
		bool sign[2] = { _ltz(lhs), _ltz(rhs), };

		if ( sign[0] && !sign[1])
			return true;

		if (!sign[0] &&  sign[1])
			return false;

		for (size_t i = 0, k = LAST; i < COUNT; ++i, --k)
		{
			if (lhs.data[k] < rhs.data[k])
				return true;
			if (lhs.data[k] > rhs.data[k])
				return false;
		}

		return false;
	}

	static bool _leq(self_t const &lhs, self_t const &rhs)
	{
		bool sign[2] = { _ltz(lhs), _ltz(rhs), };

		if ( sign[0] && !sign[1])
			return true;

		if (!sign[0] &&  sign[1])
			return false;

		for (size_t i = 0, k = LAST; i < COUNT; ++i, --k)
		{
			if (lhs.data[k] < rhs.data[k])
				return true;
			if (lhs.data[k] > rhs.data[k])
				return false;
		}

		return true;
	}

	static bool _gt(self_t const &lhs, self_t const &rhs)
	{
		bool sign[2] = { _ltz(lhs), _ltz(rhs), };

		if ( sign[0] && !sign[1])
			return false;

		if (!sign[0] &&  sign[1])
			return true;

		for (size_t i = 0, k = LAST; i < COUNT; ++i, --k)
		{
			if (lhs.data[k] > rhs.data[k])
				return true;
			if (lhs.data[k] < rhs.data[k])
				return false;
		}

		return false;
	}

	static bool _geq(self_t const &lhs, self_t const &rhs)
	{
		bool sign[2] = { _ltz(lhs), _ltz(rhs), };

		if ( sign[0] && !sign[1])
			return false;

		if (!sign[0] &&  sign[1])
			return true;

		for (size_t i = 0, k = LAST; i < COUNT; ++i, --k)
		{
			if (lhs.data[k] > rhs.data[k])
				return true;
			if (lhs.data[k] < rhs.data[k])
				return false;
		}

		return true;
	}

	static void _abs(self_t &lval, self_t const &rhs)
	{
		if (_ltz(rhs))
			_neg(lval, rhs);
		else
			_set(lval, rhs);
	}

	static void _cmp(self_t &lval, self_t const &rhs)
	{
		for (size_t i = 0; i < COUNT; ++i)
			lval.data[i] = ~rhs.data[i];
	}

	static void _neg(self_t &lval, self_t const &rhs)
	{
		self_t tmp = rhs;
		_cmp( tmp,            rhs);
		_add(lval, tmp, self_t(1));
	}

	static void _add(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		uint32_t carry = 0;
		for (size_t i = 0; i < COUNT; ++i)
		{
			uint32_t value = uint32_t(lhs.data[i] + rhs.data[i]) + carry;
			lval.data[i] = uint16_t(value & MASK);
			carry = (value >> SHIFT);
		}
	}

	static void _sub(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		self_t tmp = rhs;
		_neg( tmp,      rhs);
		_add(lval, lhs,  tmp);
	}

	static void _mul(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		self_t x;
		_abs(x, lhs);

		self_t y = rhs;
		_abs(y, rhs);

		uint32_t carry = 0;
		for (size_t i = 0; i < COUNT; ++i)
		{
			uint32_t value = carry;
			carry = 0;
			for (size_t j = 0; j <= i; ++j)
			{
				value += (x.data[j] * y.data[i - j]);
				carry += (value >> SHIFT);
				value &= MASK;
			}
			lval.data[i] = uint16_t(value);
		}

		LUCID_VALIDATE(0 == carry, "large integer overflow");

		if (_ltz(lhs) ^ _ltz(rhs))
			_neg(lval, lval);
	}

	static void _div_mod(self_t &R, self_t &Q, self_t const &lhs, self_t const &rhs)
	{
		static self_t const zero = 0;
		static self_t const  one = 1;

		LUCID_VALIDATE(_neq(zero, rhs), "large integer division by zero");

		self_t dividend;
		_abs(dividend, lhs);

		self_t divisor;
		_abs(divisor, rhs);

		Q = zero;
		R = dividend;

		self_t D;
		self_t b;

		uint16_t shift = 0;
		while (_geq(R, divisor))
		{
			D = divisor;

			shift = 0;
			while (_leq(D, R))
			{
				///	aggressively shift left until D passes R
				_lsh(D, D, 10);
				shift += 10;
			}
			while (_gt(D, R))
			{
				/// shift right by one until D passes "just below" R 
				_rsh(D, D, 1);
				shift -= 1;
			}

			_sub(R, R, D);
			_lsh(b, one, shift);
			_or (Q, Q, b);
		}

		if (_ltz(lhs) ^ _ltz(rhs))
			_neg(Q, Q);
	}

	static void _div(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		self_t R;
		_div_mod(R, lval, lhs, rhs);
	}

	static void _mod(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		self_t Q;
		_div_mod(lval, Q, lhs, rhs);
	}

	static void _and(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		for (size_t i = 0; i < COUNT; ++i)		
			lval.data[i] = lhs.data[i] & rhs.data[i];
	}

	static void _or(self_t &lval, self_t const &lhs, self_t const &rhs)
	{
		for (size_t i = 0; i < COUNT; ++i)
			lval.data[i] = lhs.data[i] | rhs.data[i];
	}

	static void _lsh(self_t &lval, self_t const &lhs, uint16_t bits)
	{
		lval = lhs;
		while (bits > 0)
		{
			uint16_t shift = LUCID_MATH::min(bits, uint16_t(SHIFT));
			bits -= shift;

			uint32_t carry = 0;
			for (size_t i = 0; i < COUNT; ++i)
			{
				uint32_t shifted = uint32_t(lval.data[i]) << shift;
				lval.data[i] = (0x0000FFFF & shifted) | carry;
				carry = (0xFFFF0000 & shifted) >> SHIFT;
			}
		}
	}

	static void _rsh(self_t &lval, self_t const &lhs, uint16_t bits)
	{
		lval = lhs;
		while (bits > 0)
		{
			uint16_t shift = LUCID_MATH::min(bits, uint16_t(SHIFT));
			bits -= shift;

			uint32_t carry = 0;
			for (size_t i = 0, k = LAST; i < COUNT; ++i, --k)
			{
				uint32_t shifted = uint32_t(lval.data[k]) << (SHIFT - shift);
				lval.data[k] = ((0xFFFF0000 & shifted) >> SHIFT) | carry;
				carry = (0x0000FFFF & shifted);
			}
		}
	}

};

///
///	expose equal and not equal methods for symmetry (matrix, vector, quaternion, scalar, etc.. all define these)
///

template<size_t N> inline bool equ(Integer<N> const lhs, Integer<N> const rhs)
{
	return Integer<N>::_equ(lhs, rhs);
}
	
template<size_t N> inline bool neq(Integer<N> const lhs, Integer<N> const rhs)
{
	return Integer<N>::_neq(lhs, rhs);
}

LUCID_MATH_END

#pragma push_macro("NUMBER")
#define NUMBER(N) ::lucid::LUCID_MATH::Integer<N>

template<size_t N> inline bool operator==(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_equ(lhs, rhs);
}

template<size_t N> inline bool operator!=(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_neq(lhs, rhs);
}

template<size_t N> inline bool operator<(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_lt(lhs, rhs);
}

template<size_t N> inline bool operator<=(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_leq(lhs, rhs);
}

template<size_t N> inline bool operator>(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_gt(lhs, rhs);
}

template<size_t N> inline bool operator>=(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	return NUMBER(N)::_geq(lhs, rhs);
}

template<size_t N> inline NUMBER(N) operator-(NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_neg(result, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator+(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_add(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator-(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_sub(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator*(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_mul(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator/(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_div(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator%(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_mod(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator&(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_and(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator|(NUMBER(N) const &lhs, NUMBER(N) const &rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_or(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator<<(NUMBER(N) const &lhs, uint16_t rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_lsh(result, lhs, rhs);
	return result;
}

template<size_t N> inline NUMBER(N) operator>>(NUMBER(N) const &lhs, uint16_t rhs)
{
	NUMBER(N) result;
	NUMBER(N)::_rsh(result, lhs, rhs);
	return result;
}

#undef NUMBER
#pragma pop_macro("NUMBER")
