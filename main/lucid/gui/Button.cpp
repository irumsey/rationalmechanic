#include "Button.h"
#include "Renderer.h"

LUCID_GUI_BEGIN

Button::Button(size_t id, ANCHOR anchor, int32_t width, int32_t height, Callback const &callback, Tiles const &tiles)
	: Frame(id, anchor, width, height)
	, _callback(callback)
	, _tiles(tiles)
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

	if (notHit(event.position))
	{
		_state = STATE_ENABLED;
		return;
	}

	if (MouseEvent::KIND_POINTER_MOVE == event.kind)
	{
		_state = STATE_FOCUSED;
		return;
	}

	if ((MouseEvent::BUTTON_LEFT == event.button) && (MouseEvent::KIND_BUTTON_DOWN == event.kind))
	{
		_state = STATE_PRESSED;
		return;
	}

	if ((MouseEvent::BUTTON_LEFT == event.button) && (MouseEvent::KIND_BUTTON_UP == event.kind))
	{
		_callback(this);
		_state = STATE_FOCUSED;
		return;
	}
}

void Button::accept(Renderer *renderer) const
{
	Point const &min = rectangle().min;
	Point const &max = rectangle().max;

	LUCID_GAL::Vector4 position(float32_t(min.x), float32_t(min.y), float32_t(max.x), float32_t(max.y));
	LUCID_GAL::Vector4 const &tile = _tiles[_state];

	Icon icon = { position, tile, LUCID_GAL::Color(1, 1, 1, 1), };
	renderer->add(icon);
}

LUCID_GUI_END
