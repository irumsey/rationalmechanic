#include "Panel.h"

LUCID_GUI_BEGIN

Panel::Panel(size_t id, ANCHOR anchor, int32_t width, int32_t height)
	: Frame(id, anchor, width, height)
{
}

void Panel::accept(Renderer *renderer) const
{
}

LUCID_GUI_END
