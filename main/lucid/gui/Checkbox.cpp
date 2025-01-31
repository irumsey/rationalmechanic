#include "Checkbox.h"
#include "Renderer.h"

LUCID_GUI_BEGIN

Checkbox::Checkbox(size_t id, ANCHOR anchor, int32_t width, int32_t height, Callback const &callback, Tiles const &tiles)
	: Frame(id, anchor, width, height)
	, _callback(callback)
	, _tiles(tiles)
{
}

void Checkbox::onEvent(TimerEvent const &event)
{
}

void Checkbox::onEvent(KeyboardEvent const &event)
{
}

void Checkbox::onEvent(MouseEvent const &event)
{
	if (!isEnabled())
	{
		return;
	}

	if (notHit(event.position))
	{
		setFocused(false);
		return;
	}

	if (MouseEvent::KIND_POINTER_MOVE == event.kind)
	{
		setFocused();
		return;
	}

	if ((MouseEvent::BUTTON_LEFT == event.button) && (MouseEvent::KIND_BUTTON_UP == event.kind))
	{
		toggleChecked();
		_callback(this);
		return;
	}
}

void Checkbox::accept(Renderer *renderer) const
{
	Point const &min = rectangle().min;
	Point const &max = rectangle().max;

	LUCID_GAL::Vector4 position(float32_t(min.x), float32_t(min.y), float32_t(max.x), float32_t(max.y));
	LUCID_GAL::Vector4 const &tile = _tiles[_state];

	Icon icon = { position, tile, LUCID_GAL::Color(1, 1, 1, 1), };
	renderer->add(icon);
}

LUCID_GUI_END
