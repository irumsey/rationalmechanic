#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

LUCID_ORBIT_BEGIN

///	Orbital Elements
///
///
struct Elements
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
/// Note: just starting the process of representing
/// the rotation of objects.  for now, just going to
/// represent angular position and velocity at a given
/// moment of time. 
/// In the future, i imagine adding a generalized 
/// moment of inertia matrix into physical properties
/// and extending this for more generalized simulations.
/// (esp. for dynamic bodies)
struct RotationalElements
{
	scalar_t	JDN = 0.0;	/// Julian day number
	vector3_t     A;		///	Axis of rotation
	scalar_t  theta = 0.0;	/// Angular position (radians)
	scalar_t      w = 0.0;	///	Angular velocity (radians / day)
};

LUCID_ORBIT_END
