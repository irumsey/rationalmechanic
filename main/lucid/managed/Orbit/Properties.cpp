#include "Properties.h"

namespace /* anonymous */ {

	namespace orbit = ::lucid::orbit;

}

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	PhysicalProperties::PhysicalProperties(orbit::PhysicalProperties const &properties)
	{
		_internal = new orbit::PhysicalProperties(properties);
		_isOwner = true;
	}

	PhysicalProperties::PhysicalProperties(orbit::PhysicalProperties *properties)
		: _internal(properties)
		, _isOwner(false)
	{
	}

	PhysicalProperties::~PhysicalProperties()
	{
		this->!PhysicalProperties();
	}

	PhysicalProperties::!PhysicalProperties()
	{
		if (_isOwner)
			delete _internal;
	}

	/// 
	/// 
	/// 
	
	RenderProperties::RenderProperties(orbit::RenderProperties *properties)
		: _internal(properties)
	{
	}

	RenderProperties::~RenderProperties()
	{
		this->!RenderProperties();
	}

	RenderProperties::!RenderProperties()
	{
		/// does not own _internal
	}

}	///	Orbit
}	///	Lucid