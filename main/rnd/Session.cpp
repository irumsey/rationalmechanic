#include "Session.h"
#include "State.h"
#include <lucid/gui/Frame.h>
#include <cassert>

#ifdef min
#	undef min
#endif

Session::Session()
{
	_state = Stopped::instance();
	_state->onEnter(this);
}

void Session::initialize(LUCID_GUI::Rectangle const &rectangle)
{
	_rectangle = rectangle;
	_guiRender.initialize("content/OCRa.font", "content/gui.material");

	changeState(Starting::instance());
}

void Session::shutdown()
{
	changeState(Stopping::instance());

	_guiRender.shutdown();
}

void Session::onEvent(LUCID_GUI::TimerEvent const &event)
{
	assert(nullptr != _state);

	_state->onEvent(this, event);
}

void Session::onEvent(LUCID_GUI::SizeEvent const &event)
{
	assert(nullptr != _state);

	_rectangle = event.rectangle;
	_state->onEvent(this, event);
}

void Session::onEvent(LUCID_GUI::MouseEvent const &event)
{
	assert(nullptr != _state);

	LUCID_GUI::MouseEvent flipped = event;
	flipped.position.y = _rectangle.max.y - event.position.y;

	_state->onEvent(this, flipped);
}

void Session::onButtonPress(LUCID_GUI::Button *button)
{
	assert(nullptr != _state);

	_state->onButtonPress(this, button);
}

void Session::onCheckboxPress(LUCID_GUI::Checkbox *button)
{
	assert(nullptr != _state);

	_state->onCheckboxPress(this, button);
}

void Session::render(float64_t time)
{
	assert(nullptr != _state);
	_state->render(this, time);
}

inline void Session::changeState(State const *state)
{
	assert(nullptr != _state);
	assert(nullptr !=  state);

	_state->onLeave(this);
	_state = state;
	_state->onEnter(this);
}
