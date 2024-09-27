#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Elements.h>
#include <lucid/managed/Core/Utility.h>

namespace Lucid {
namespace Orbit{

	///
	///
	///
	public ref class OrbitalElements
	{
	public:
		OrbitalElements(LUCID_ORBIT::OrbitalElements const &elements);

		~OrbitalElements();

		!OrbitalElements();

		LUCID_MANAGED_PROPERTY( JDN, double)	///	Julian day number
		LUCID_MANAGED_PROPERTY(  EC, double)	/// Eccentricity
		LUCID_MANAGED_PROPERTY(  QR, double)	/// Periapsis distance
		LUCID_MANAGED_PROPERTY(  IN, double)	/// Inclination
		LUCID_MANAGED_PROPERTY(  OM, double)	/// Longitude of Ascending Node
		LUCID_MANAGED_PROPERTY(   W, double)	/// Argument of Perifocus
		LUCID_MANAGED_PROPERTY(  Tp, double)	/// Time of periapsis
		LUCID_MANAGED_PROPERTY(   N, double)	/// Mean motion
		LUCID_MANAGED_PROPERTY(  MA, double)	/// Mean anomaly
		LUCID_MANAGED_PROPERTY(  TA, double)	/// True anomaly
		LUCID_MANAGED_PROPERTY(   A, double)	/// Semi-major axis
		LUCID_MANAGED_PROPERTY(  AD, double)	/// Apoapsis distance
		LUCID_MANAGED_PROPERTY(  PR, double)	/// Sidereal orbit period

		property LUCID_ORBIT::OrbitalElements const &ref { LUCID_ORBIT::OrbitalElements const &get() { return *ptr; } }

		property LUCID_ORBIT::OrbitalElements const *ptr { LUCID_ORBIT::OrbitalElements const *get() { return _internal; } }

	private:
		LUCID_ORBIT::OrbitalElements *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
