#pragma once

#include <lucid/core/Types.h>

///
///
///

#define LUCID_CONSTANT(name,value)					\
	template<class T> static T name()				\
	{												\
		static_assert(false, "bad cast");			\
		return T();									\
	}												\
	template<> static float32_t name()				\
	{												\
		static float32_t const result = value##F;	\
		return result;								\
	}												\
	template<> static float64_t name()				\
	{												\
		static float64_t const result = value;		\
		return result;								\
	}												\
	template<> static float128_t name()				\
	{												\
		static float128_t const result = value##L;	\
		return result;								\
	}
