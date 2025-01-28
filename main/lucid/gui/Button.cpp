#include "Button.h"

LUCID_ANONYMOUS_BEGIN

void ignore(LUCID_GUI::Button *) {}

LUCID_ANONYMOUS_END

LUCID_GUI_BEGIN

Button::Button(size_t id, ANCHOR anchor, int32_t width, int32_t height)
	: Frame(id, anchor, width, height)
	, _callback(ignore)
{
}

void Button::onEvent(TimerEvent const &event)
{
}

void Button::onEvent(KeyboardEvent const &event)
{
}

void Button::onEvent(MouseEvent const &event)
{
	if (STATE_DISABLED == _state)
	{
		return;
	}

	bool hit = isHit(event.position);

	if (MouseEvent::KIND_POINTER_MOVE == event.kind)
	{
		_state = hit ? STATE_FOCUSED : STATE_ENABLED;
		return;
	}

	if (hit && (MouseEvent::KIND_BUTTON_UP == event.kind) && (MouseEvent::BUTTON_LEFT == event.button))
	{
		_callback(this);
	}
}

LUCID_GUI_END
