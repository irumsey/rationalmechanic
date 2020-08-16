#pragma once

#include <lucid/orbit/Types.h>

namespace lucid {
namespace orbit {

	///	Orbital elements
	///
	///
	struct Elements
	{
		scalar_t JDN = 0.f;	///	Julian day number
		scalar_t  EC = 0.f;	/// Eccentricity
		scalar_t  QR = 0.f;	/// Periapsis distance
		scalar_t  IN = 0.f;	/// Inclination
		scalar_t  OM = 0.f;	/// Longitude of Ascending Node
		scalar_t   W = 0.f;	/// Argument of Perifocus
		scalar_t  Tp = 0.f;	/// Time of periapsis
		scalar_t   N = 0.f;	/// Mean motion
		scalar_t  MA = 0.f;	/// Mean anomaly
		scalar_t  TA = 0.f;	/// True anomaly
		scalar_t   A = 0.f;	/// Semi-major axis
		scalar_t  AD = 0.f;	/// Apoapsis distance
		scalar_t  PR = 0.f;	/// Sidereal orbit period
	};

}	///	orbit
}	///	lucid
