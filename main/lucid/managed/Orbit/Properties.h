#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Properties.h>
#include <lucid/managed/Core/Utility.h>

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

		LUCID_MANAGED_PROPERTY_EX(     GM,     GM, double)
		LUCID_MANAGED_PROPERTY_EX(   Mass,   mass, double)
		LUCID_MANAGED_PROPERTY_EX( Radius, radius, double)

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

		LUCID_MANAGED_PROPERTY_EX(     ShowOrbit,      showOrbit, bool)
		LUCID_MANAGED_PROPERTY_EX(OrbitHighlight, orbitHighlight, bool)
		LUCID_MANAGED_PROPERTY_EX( BodyHighlight,  bodyHighlight, bool)

		property LUCID_ORBIT::RenderProperties const &ref { LUCID_ORBIT::RenderProperties const &get() { return *ptr; } }

		property LUCID_ORBIT::RenderProperties const *ptr { LUCID_ORBIT::RenderProperties const *get() { return _internal; } }

	private:
		bool _isOwner = false;
		LUCID_ORBIT::RenderProperties *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
