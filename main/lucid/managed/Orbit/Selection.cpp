#include "Selection.h"

namespace /* anonymous */ {

	namespace orbit = ::lucid::orbit;

}

namespace Lucid {
namespace Orbit {

	Selection::Selection(orbit::Selection const &selection)
	{
		_internal = new orbit::Selection(selection);
	}

	Selection::~Selection()
	{
		this->!Selection();
	}

	Selection::!Selection()
	{
		delete _internal;
	}

}	///	Orbit
}	///	Lucid