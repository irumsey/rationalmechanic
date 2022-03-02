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
		PhysicalProperties(::lucid::orbit::PhysicalProperties const &properties);

		~PhysicalProperties();

		!PhysicalProperties();

		PROPERTY(     GM,     GM, double)
		PROPERTY(   Mass,   mass, double)
		PROPERTY( Radius, radius, double)

		property ::lucid::orbit::PhysicalProperties const &ref { ::lucid::orbit::PhysicalProperties const &get() { return *ptr; } }

		property ::lucid::orbit::PhysicalProperties const *ptr { ::lucid::orbit::PhysicalProperties const *get() { return _internal; } }

	private:
		::lucid::orbit::PhysicalProperties *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
