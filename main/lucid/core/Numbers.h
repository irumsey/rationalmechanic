#pragma once

///
///
/// 

#include <lucid/core/Types.h>

///
///
///

namespace lucid {
namespace core {
	template<typename T> struct invalid_type {};
}
}

#define LUCID_NUMBER(name, value)													\
	template<typename T> constexpr T name = ::lucid::core::invalid_type<T>();		\
	template<> constexpr float32_t name<float32_t> = float32_t(value);				\
	template<> constexpr float64_t name<float64_t> = float64_t(value);				\
	template<> constexpr float128_t name<float128_t> = float128_t(value)

namespace lucid {
namespace core {
namespace numbers {

	LUCID_NUMBER( e, 2.718281828459045235360287471352);
	LUCID_NUMBER(pi, 3.141592653589793238462643383279);

}
}
}
