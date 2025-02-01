#include "Label.h"
#include "Renderer.h"

LUCID_GUI_BEGIN

Label::Label(size_t id, ANCHOR anchor, Size const &size, ALIGNMENT align, std::string const &text, Color const &color)
	: Frame(id, anchor, size)
	, _alignment(align)
	, _text(text)
	, _color(color)
{
}

void Label::accept(Renderer *renderer) const
{
	Rectangle const &rect = rectangle();
	float32_t size = float32_t(rect.max.y - rect.min.y);
	
	Point point = Point(rect.min.x, rect.min.y);
	switch (_alignment)
	{
	case ALIGNMENT::ALIGN_RIGHT:
		point.x = rect.max.x;
		break;
	case ALIGNMENT::ALIGN_CENTER:
		point.x += (rect.max.x - rect.min.x) >> 1;
		break;
	default:
		break;
	};
	 
	renderer->add(_alignment, point, size, _text, _color);
}

LUCID_GUI_END
