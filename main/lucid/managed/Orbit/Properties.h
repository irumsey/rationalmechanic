#pragma once

#include <lucid/orbit/Defines.h>
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
		PhysicalProperties(LUCID_ORBIT::PhysicalProperties *properties);

		PhysicalProperties(LUCID_ORBIT::PhysicalProperties const &properties);

		~PhysicalProperties();

		!PhysicalProperties();

		PROPERTY(     GM,     GM, double)
		PROPERTY(   Mass,   mass, double)
		PROPERTY( Radius, radius, double)

		property LUCID_ORBIT::PhysicalProperties const &ref { LUCID_ORBIT::PhysicalProperties const &get() { return *ptr; } }

		property LUCID_ORBIT::PhysicalProperties const *ptr { LUCID_ORBIT::PhysicalProperties const *get() { return _internal; } }

	private:
		bool _isOwner = false;
		LUCID_ORBIT::PhysicalProperties *_internal = nullptr;

	};

	///
	///
	/// 
	public ref class RenderProperties
	{
	public:
		RenderProperties(LUCID_ORBIT::RenderProperties *properties);

		RenderProperties(LUCID_ORBIT::RenderProperties const &properties);

		~RenderProperties();

		!RenderProperties();

		PROPERTY(     ShowOrbit,      showOrbit, bool)
		PROPERTY(OrbitHighlight, orbitHighlight, bool)
		PROPERTY( BodyHighlight,  bodyHighlight, bool)

		property LUCID_ORBIT::RenderProperties const &ref { LUCID_ORBIT::RenderProperties const &get() { return *ptr; } }

		property LUCID_ORBIT::RenderProperties const *ptr { LUCID_ORBIT::RenderProperties const *get() { return _internal; } }

	private:
		bool _isOwner = false;
		LUCID_ORBIT::RenderProperties *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
