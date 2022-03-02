#include "Properties.h"

namespace /* anonymous */ {

	namespace orbit = ::lucid::orbit;

}

namespace Lucid {
namespace Orbit {

	PhysicalProperties::PhysicalProperties(orbit::PhysicalProperties const &properties)
	{
		_internal = new orbit::PhysicalProperties(properties);
	}

	PhysicalProperties::~PhysicalProperties()
	{
		this->!PhysicalProperties();
	}

	PhysicalProperties::!PhysicalProperties()
	{
		delete _internal;
	}

}	///	Orbit
}	///	Lucid