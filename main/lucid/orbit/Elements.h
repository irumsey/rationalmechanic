#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

LUCID_ORBIT_BEGIN

///	OrbitalElements
///
///
struct OrbitalElements
{
	scalar_t JDN = 0.0;	///	Julian day number
	scalar_t  EC = 0.0;	/// Eccentricity
	scalar_t  QR = 0.0;	/// Periapsis distance
	scalar_t  IN = 0.0;	/// Inclination
	scalar_t  OM = 0.0;	/// Longitude of Ascending Node
	scalar_t   W = 0.0;	/// Argument of Perifocus
	scalar_t  Tp = 0.0;	/// Time of periapsis
	scalar_t   N = 0.0;	/// Mean motion
	scalar_t  MA = 0.0;	/// Mean anomaly
	scalar_t  TA = 0.0;	/// True anomaly
	scalar_t   A = 0.0;	/// Semi-major axis
	scalar_t  AD = 0.0;	/// Apoapsis distance
	scalar_t  PR = 0.0;	/// Sidereal orbit period
};

///	RotationalElements
///
/// Note: each of the elements are coefficients.
/// Usage: angle = c[0] + c[1] * t + c[2] * t**2
/// Where: t is julian centuries since J2000.
struct RotationalElements
{
	scalar_t  ra[3] = { 0, 0, 0, };
	scalar_t dec[3] = { 0, 0, 0, };
	scalar_t  pm[3] = { 0, 0, 0, };

};

LUCID_ORBIT_END
