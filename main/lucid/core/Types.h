#pragma once

///
///
///

#include <cinttypes>

///
///
///

typedef unsigned short byte_t;

///
///	TBD: watch out for complier specific implementations
///

typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;

namespace lucid {
namespace core {

	///	Flag
	///
	///	a simple flag type which will default to either true or false.
	///	intended for use with std::map and std::unordered_map where first call to:
	///		bool value = map[K];
	///	might result in uninitialized data.
	template<bool DEFAULT> struct Flag
	{
		bool value = DEFAULT;

		Flag() = default;

		Flag(bool rhs)
			: value(rhs)
		{
		}

		operator bool () const
		{
			return value;
		}

		Flag &operator=(bool rhs)
		{
			value = rhs;
			return *this;
		}
	};

}	///	core
}	///	lucid