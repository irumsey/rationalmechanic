#pragma once

#include <lucid/orbit/Elements.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(name, type) property type name { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit{

	///
	///
	///
	public ref class Elements
	{
	public:
		Elements(::lucid::orbit::Elements const &elements);

		~Elements();

		!Elements();

		PROPERTY( JDN, double)	///	Julian day number
		PROPERTY(  EC, double)	/// Eccentricity
		PROPERTY(  QR, double)	/// Periapsis distance
		PROPERTY(  IN, double)	/// Inclination
		PROPERTY(  OM, double)	/// Longitude of Ascending Node
		PROPERTY(   W, double)	/// Argument of Perifocus
		PROPERTY(  Tp, double)	/// Time of periapsis
		PROPERTY(   N, double)	/// Mean motion
		PROPERTY(  MA, double)	/// Mean anomaly
		PROPERTY(  TA, double)	/// True anomaly
		PROPERTY(   A, double)	/// Semi-major axis
		PROPERTY(  AD, double)	/// Apoapsis distance
		PROPERTY(  PR, double)	/// Sidereal orbit period

		property ::lucid::orbit::Elements const &ref { ::lucid::orbit::Elements const &get() { return *ptr; } }

		property ::lucid::orbit::Elements const *ptr { ::lucid::orbit::Elements const *get() { return _internal; } }

	private:
		::lucid::orbit::Elements *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid


#undef PROPERTY
#pragma pop_macro("PROPERTY")
