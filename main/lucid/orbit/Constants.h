#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Numbers.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

LUCID_ORBIT_BEGIN

template<typename T> struct invalid_type {};

LUCID_ORBIT_END

#define LUCID_ORBIT_CONSTANT(name, value, units)																								\
	template<typename T> LUCID_MATH::Scalar<T, units> const name = ::lucid::orbit::invalid_type<T>();										\
	template<> LUCID_MATH::Scalar<float32_t, units> const name<float32_t> = LUCID_MATH::Scalar<float32_t, units>(float32_t(value));		\
	template<> LUCID_MATH::Scalar<float64_t, units> const name<float64_t> = LUCID_MATH::Scalar<float64_t, units>(float64_t(value));		\
	template<> LUCID_MATH::Scalar<float128_t, units> const name<float128_t> = LUCID_MATH::Scalar<float128_t, units>(float128_t(value))

LUCID_ORBIT_CONSTANTS_BEGIN

///	 AU = astronomical unit
///	 RU = render unit

LUCID_ORBIT_CONSTANT(     e, 2.718281828459045235360287471352, unit_of_measure);
LUCID_ORBIT_CONSTANT(two_pi, 6.283185307179586476925286766559, unit_of_measure);
LUCID_ORBIT_CONSTANT(    pi, 3.141592653589793238462643383279, unit_of_measure);

LUCID_ORBIT_CONSTANT(          J2000,                               2451544.0,      unit_of_measure);
LUCID_ORBIT_CONSTANT(seconds_per_day,                                 86400.0,      unit_of_measure);
LUCID_ORBIT_CONSTANT(  meters_per_AU,                          149597870700.0,      unit_of_measure);
LUCID_ORBIT_CONSTANT(  AUs_per_meter,   6.6845871222684454959959533702106e-12,      unit_of_measure);
LUCID_ORBIT_CONSTANT(  meters_per_RU,                            1495978707.0,    LUCID_UNITS::none);
LUCID_ORBIT_CONSTANT(  RUs_per_meter,   6.6845871222684454959959533702106e-10,      unit_of_measure);

LUCID_ORBIT_CONSTANT(  tol, 0.00010000, unit_of_measure);
LUCID_ORBIT_CONSTANT(tolsq, 0.00000001, unit_of_measure);

LUCID_ORBIT_CONSTANTS_END