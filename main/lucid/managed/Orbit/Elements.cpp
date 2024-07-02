#include "Elements.h"

namespace Lucid {
namespace Orbit {

	Elements::Elements(LUCID_ORBIT::Elements const &elements)
	{
		_internal = new LUCID_ORBIT::Elements(elements);
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