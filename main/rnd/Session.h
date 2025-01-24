#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Frame.h>
#include <rnd/Utility.h>

class State;

/// 
/// 
/// 
class Session
{
public:
	Session();

	virtual ~Session() = default;

	void initialize();

	void shutdown();

	void onMouseButton(MOUSE_BUTTON button, bool down, point2d_t const &point);

	void onMouseWheel(int32_t delta);

	void onMouseMove(point2d_t const &point);

	void update(float64_t t, float32_t dt);

	void render(float64_t t, float32_t interpolant);

private:
	friend class Stopped;
	friend class Starting;
	friend class Simulating;

	State const *state = nullptr;
	void changeState(State const *newState);

	LUCID_GUI::Frame *gui = nullptr;

	Session &ref();

	Session *ptr();

	LUCID_PREVENT_COPY(Session);
	LUCID_PREVENT_ASSIGNMENT(Session);
};

inline Session &Session::ref()
{
	return *this;
}

inline Session *Session::ptr()
{
	return this;
}
