#include "Panel.h"

LUCID_GUI_BEGIN

Panel::Panel(uint32_t id, ANCHOR anchor, Size const &size)
	: Frame(id, anchor, size)
{
}

void Panel::accept(Renderer *renderer) const
{
}

LUCID_GUI_END
