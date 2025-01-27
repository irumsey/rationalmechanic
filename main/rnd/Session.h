#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Types.h>
#include <lucid/gui/Events.h>
#include <rnd/Utility.h>

LUCID_GUI_BEGIN

class Frame;

LUCID_GUI_END

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

	void onEvent(LUCID_GUI::MouseEvent const &event);

	void update(float64_t t, float32_t dt);

	void render(float64_t t, float32_t interpolant);

private:
	friend class Stopped;
	friend class Starting;
	friend class Stopping;
	friend class Configuring;
	friend class Running;

	State const *_state = nullptr;
	void changeState(State const *state);

	LUCID_GUI::Frame *_gui = nullptr;

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
