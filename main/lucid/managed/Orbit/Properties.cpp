#include "Properties.h"

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	PhysicalProperties::PhysicalProperties(LUCID_ORBIT::PhysicalProperties const &properties)
	{
		_internal = new LUCID_ORBIT::PhysicalProperties(properties);
		_isOwner = true;
	}

	PhysicalProperties::PhysicalProperties(LUCID_ORBIT::PhysicalProperties *properties)
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
	
	RenderProperties::RenderProperties(LUCID_ORBIT::RenderProperties const &properties)
	{
		_internal = new LUCID_ORBIT::RenderProperties(properties);
		_isOwner = true;
	}

	RenderProperties::RenderProperties(LUCID_ORBIT::RenderProperties *properties)
		: _internal(properties)
		, _isOwner(false)
	{
	}

	RenderProperties::~RenderProperties()
	{
		this->!RenderProperties();
	}

	RenderProperties::!RenderProperties()
	{
		if (_isOwner)
			delete _internal;
	}


}	///	Orbit
}	///	Lucid