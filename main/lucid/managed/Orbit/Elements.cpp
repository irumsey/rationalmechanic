#include "Elements.h"

namespace Lucid {
namespace Orbit {

	OrbitalElements::OrbitalElements(LUCID_ORBIT::OrbitalElements const &elements)
	{
		_internal = new LUCID_ORBIT::OrbitalElements(elements);
	}

	OrbitalElements::~OrbitalElements()
	{
		this->!OrbitalElements();
	}

	OrbitalElements::!OrbitalElements()
	{
		delete _internal;
	}

}	///	Orbit
}	///	Lucid