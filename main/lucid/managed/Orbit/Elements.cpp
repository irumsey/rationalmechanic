#include "Elements.h"

namespace /* anonymous */ {

	namespace orbit = ::lucid::orbit;
}

namespace Lucid {
namespace Orbit {

	Elements::Elements(orbit::Elements const &elements)
	{
		_internal = new orbit::Elements(elements);
	}

	Elements::~Elements()
	{
		this->!Elements();
	}

	Elements::!Elements()
	{
		delete _internal;
	}

}	///	Orbit
}	///	Lucid