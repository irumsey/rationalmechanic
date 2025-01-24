#include "Session.h"
#include "State.h"
#include <cassert>

#ifdef min
#	undef min
#endif

Session::Session()
{
	state = Stopped::instance();
}

void Session::initialize()
{
	shutdown();
	changeState(Starting::instance());
}

void Session::shutdown()
{
	changeState(Stopped::instance());
}

void Session::onMouseButton(MOUSE_BUTTON button, bool down, point2d_t const &point)
{
	assert(nullptr != state);

	state->onMouseButton(ref(), button, down, point);
}

void Session::onMouseWheel(int32_t delta)
{
	assert(nullptr != state);

	state->onMouseWheel(ref(), delta);
}

void Session::onMouseMove(point2d_t const &point)
{
	assert(nullptr != state);

	state->onMouseMove(ref(), point);
}

void Session::update(float64_t t, float32_t dt)
{
	assert(nullptr != state);

	state->update(ref(), t, dt);
}

void Session::render(float64_t t, float32_t interpolant)
{
	assert(nullptr != state);

	state->render(ref(), t, interpolant);
}

inline void Session::changeState(State const *newState)
{
	assert(nullptr != state);

	state->onLeave(ref());
	state = newState;
	state->onEnter(ref());
}
