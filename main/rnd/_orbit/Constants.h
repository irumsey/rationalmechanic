#pragma once

#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>

#define ORBIT_CONSTANTS_BEGIN ORBIT_BEGIN namespace constants {
#define ORBIT_CONSTANTS_END } ORBIT_END

#define ORBIT_CONSTANTS ORBIT::constants

#define ORBIT_CONSTANT(name, value) scalar_t const name = scalar_t(value);

ORBIT_CONSTANTS_BEGIN

ORBIT_CONSTANT(     e, 2.718281828459045235360287471352);
ORBIT_CONSTANT(two_pi, 6.283185307179586476925286766559);
ORBIT_CONSTANT(    pi, 3.141592653589793238462643383279);

ORBIT_CONSTANT(   tol, 0.00010000);
ORBIT_CONSTANT( tolsq, 0.00000001);

ORBIT_CONSTANT(             J2000,                               2451545.0);
ORBIT_CONSTANT(   seconds_per_day,                                 86400.0);
ORBIT_CONSTANT(   days_per_second,							 1.0 / 86400.0);
ORBIT_CONSTANT(  days_per_century,								   36525.0);
ORBIT_CONSTANT(     meters_per_AU,                          149597870700.0);
ORBIT_CONSTANT(     AUs_per_meter,   6.6845871222684454959959533702106e-12);
ORBIT_CONSTANT(      rads_per_deg,                              pi / 180.0);
ORBIT_CONSTANT(      degs_per_rad,                              180.0 / pi);

ORBIT_CONSTANTS_END
