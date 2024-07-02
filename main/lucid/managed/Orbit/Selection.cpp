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

	SelectionType Selection::Type::get()
	{
		return SelectionType(_internal->type);
	}

	void Selection::Type::set(SelectionType value)
	{
		_internal->type = LUCID_ORBIT::Selection::TYPE(value);
	}

}	///	Orbit
}	///	Lucid