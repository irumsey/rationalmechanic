#include "Iterator.h"

LUCID_XPR_BEGIN

Iterator::Iterator(Node const *node)
{
	set(node);
}

Iterator Iterator::operator++(int)
{
	Iterator temp;

	temp._current = _current;
	temp._stack = _stack;

	_current->apply(this);

	return temp;
}

LUCID_XPR_END
