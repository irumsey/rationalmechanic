#include "Session.h"
#include "SessionState.h"

void Session::setState(SessionState *state)
{
	if (nullptr != _state)
	{
		_state->onLeave(this);
	}

	_state = state;

	if (nullptr != _state)
	{
		_state->onEnter(this);
	}
}

void Session::onInput(MouseEvent const &event)
{
	if (nullptr != _state)
	{
		_state->onInput(this, event);
	}
}

void Session::onUpdate(float64_t t, float64_t dt)
{
	if (nullptr != _state)
	{
		_state->onUpdate(this, t, dt);
	}
}

void Session::onRender(float32_t t, float32_t interpolant)
{
	if (nullptr != _state)
	{
		_state->onRender(this, t, interpolant);
	}
}
