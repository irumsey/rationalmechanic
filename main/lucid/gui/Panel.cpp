#include "Panel.h"

LUCID_GUI_BEGIN

Panel::Panel(size_t id, ANCHOR anchor, Size const &size)
	: Frame(id, anchor, size)
{
}

void Panel::accept(Renderer *renderer) const
{
}

LUCID_GUI_END
