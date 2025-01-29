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
	_guiRender.initialize();

	changeState(Starting::instance());
}

void Session::shutdown()
{
	changeState(Stopping::instance());

	_guiRender.shutdown();
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

void Session::update(float64_t t, float32_t dt)
{
	assert(nullptr != _state);
	_state->update(this, t, dt);
}

void Session::render(float64_t t, float32_t interpolant)
{
	assert(nullptr != _state);
	_state->render(this, t, interpolant);
}

inline void Session::changeState(State const *state)
{
	assert(nullptr != _state);
	assert(nullptr !=  state);

	_state->onLeave(this);
	_state = state;
	_state->onEnter(this);
}
