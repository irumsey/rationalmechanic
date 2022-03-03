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

	SelectionType Selection::Type::get()
	{
		return SelectionType(_internal->type);
	}

	void Selection::Type::set(SelectionType value)
	{
		_internal->type = orbit::Selection::TYPE(value);
	}

}	///	Orbit
}	///	Lucid