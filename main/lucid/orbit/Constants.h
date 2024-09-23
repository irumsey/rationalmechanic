#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

#define LUCID_ORBIT_CONSTANTS_BEGIN LUCID_ORBIT_BEGIN namespace constants {
#define LUCID_ORBIT_CONSTANTS_END } LUCID_ORBIT_END

#define LUCID_ORBIT_CONSTANTS LUCID_ORBIT::constants

#define LUCID_ORBIT_CONSTANT(name, value)									\
	template<typename T> T const name = invalid_type<T>();					\
	template<>   float32_t const name< float32_t> =  float32_t(value);		\
	template<>   float64_t const name< float64_t> =  float64_t(value);		\
	template<>  float128_t const name<float128_t> = float128_t(value);

LUCID_ORBIT_CONSTANTS_BEGIN

template<typename T> struct invalid_type {};

LUCID_ORBIT_CONSTANT(     e, 2.718281828459045235360287471352);
LUCID_ORBIT_CONSTANT(two_pi, 6.283185307179586476925286766559);
LUCID_ORBIT_CONSTANT(    pi, 3.141592653589793238462643383279);

LUCID_ORBIT_CONSTANT(   tol, 0.00010000);
LUCID_ORBIT_CONSTANT( tolsq, 0.00000001);

LUCID_ORBIT_CONSTANT(             J2000,                               2451545.0);
LUCID_ORBIT_CONSTANT(   seconds_per_day,                                 86400.0);
LUCID_ORBIT_CONSTANT(     meters_per_AU,                          149597870700.0);
LUCID_ORBIT_CONSTANT(     AUs_per_meter,   6.6845871222684454959959533702106e-12);

LUCID_ORBIT_CONSTANTS_END