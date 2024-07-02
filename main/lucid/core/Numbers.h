#pragma once

#include <lucid/core/Defines.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

template<typename T> struct invalid_type {};

LUCID_CORE_END

#define LUCID_NUMBER(name, value)													\
	template<typename T> constexpr T name = LUCID_CORE::invalid_type<T>();			\
	template<> constexpr float32_t name<float32_t> = float32_t(value);				\
	template<> constexpr float64_t name<float64_t> = float64_t(value);				\
	template<> constexpr float128_t name<float128_t> = float128_t(value)

#define LUCID_CORE_NUMBERS_BEGIN LUCID_CORE_BEGIN namespace numbers {
#define LUCID_CORE_NUMBERS_END } LUCID_CORE_END

LUCID_CORE_NUMBERS_BEGIN

LUCID_NUMBER( e, 2.718281828459045235360287471352);
LUCID_NUMBER(pi, 3.141592653589793238462643383279);

LUCID_CORE_NUMBERS_END