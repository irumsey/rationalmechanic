#pragma once

#include <lucid/orbit/Properties.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(alias, name, type) property type alias { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit{

	///
	///
	///
	public ref class PhysicalProperties
	{
	public:
		PhysicalProperties(::lucid::orbit::PhysicalProperties *properties);

		PhysicalProperties(::lucid::orbit::PhysicalProperties const &properties);

		~PhysicalProperties();

		!PhysicalProperties();

		PROPERTY(     GM,     GM, double)
		PROPERTY(   Mass,   mass, double)
		PROPERTY( Radius, radius, double)

		property ::lucid::orbit::PhysicalProperties const &ref { ::lucid::orbit::PhysicalProperties const &get() { return *ptr; } }

		property ::lucid::orbit::PhysicalProperties const *ptr { ::lucid::orbit::PhysicalProperties const *get() { return _internal; } }

	private:
		bool _isOwner = false;
		::lucid::orbit::PhysicalProperties *_internal = nullptr;

	};

	///
	///
	/// 
	public ref class RenderProperties
	{
	public:
		RenderProperties(::lucid::orbit::RenderProperties *properties);

		RenderProperties(::lucid::orbit::RenderProperties const &properties);

		~RenderProperties();

		!RenderProperties();

		PROPERTY(     ShowOrbit,      showOrbit, bool)
		PROPERTY(OrbitHighlight, orbitHighlight, bool)
		PROPERTY( BodyHighlight,  bodyHighlight, bool)

		property ::lucid::orbit::RenderProperties const &ref { ::lucid::orbit::RenderProperties const &get() { return *ptr; } }

		property ::lucid::orbit::RenderProperties const *ptr { ::lucid::orbit::RenderProperties const *get() { return _internal; } }

	private:
		bool _isOwner = false;
		::lucid::orbit::RenderProperties *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
