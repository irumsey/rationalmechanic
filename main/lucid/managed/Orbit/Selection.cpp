#include "Selection.h"

namespace Lucid {
namespace Orbit {

	Selection::Selection(LUCID_ORBIT::Selection const &selection)
	{
		_internal = new LUCID_ORBIT::Selection(selection);
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