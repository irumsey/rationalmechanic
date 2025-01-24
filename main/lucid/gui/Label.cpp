#include "Label.h"

LUCID_GUI_BEGIN

Label::Label(ANCHOR anchor, int32_t width, int32_t height, std::string const &text)
	: Frame(anchor, width, height)
	, text(text)
{
}

LUCID_GUI_END
