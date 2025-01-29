#include "Label.h"

LUCID_GUI_BEGIN

Label::Label(size_t id, ANCHOR anchor, int32_t width, int32_t height)
	: Frame(id, anchor, width, height)
{
}

void Label::accept(Renderer *renderer) const
{
}

LUCID_GUI_END
