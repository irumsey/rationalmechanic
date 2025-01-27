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
	_state->onEnter(ref());
}

void Session::initialize()
{
	changeState(Starting::instance());
}

void Session::shutdown()
{
	changeState(Stopping::instance());
}

void Session::onEvent(LUCID_GUI::MouseEvent const &event)
{
	assert(nullptr != _state);
	_state->onEvent(ref(), event);
}

void Session::update(float64_t t, float32_t dt)
{
	assert(nullptr != _state);
	_state->update(ref(), t, dt);
}

void Session::render(float64_t t, float32_t interpolant)
{
	assert(nullptr != _state);
	_state->render(ref(), t, interpolant);
}

inline void Session::changeState(State const *state)
{
	assert(nullptr != _state);
	assert(nullptr !=  state);

	_state->onLeave(ref());
	_state = state;
	_state->onEnter(ref());
}
