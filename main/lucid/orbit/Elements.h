#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Types.h>

namespace lucid {
namespace orbit {

	///	Orbital elements
	///
	///
	struct Elements
	{
		float32_t JDN = 0.f;	///	Julian day number
		float32_t  EC = 0.f;	/// Eccentricity
		float32_t  QR = 0.f;	/// Periapsis distance
		float32_t  IN = 0.f;	/// Inclination
		float32_t  OM = 0.f;	/// Longitude of Ascending Node
		float32_t   W = 0.f;	/// Argument of Perifocus
		float32_t  Tp = 0.f;	/// Time of periapsis
		float32_t   N = 0.f;	/// Mean motion
		float32_t  MA = 0.f;	/// Mean anomaly
		float32_t  TA = 0.f;	/// True anomaly
		float32_t   A = 0.f;	/// Semi-major axis
		float32_t  AD = 0.f;	/// Apoapsis distance
		float32_t  PR = 0.f;	/// Sidereal orbit period
	};

}	///	orbit
}	///	lucid