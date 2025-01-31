#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gigl/Context.h>
#include <lucid/gui/Types.h>
#include <lucid/gui/Events.h>
#include <lucid/gui/Renderer.h>
#include <rnd/Utility.h>

LUCID_GUI_BEGIN

class Frame;
class Button;
class Checkbox;

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

	void initialize(LUCID_GUI::Rectangle const &rectangle);

	void shutdown();

	void onEvent(LUCID_GUI::SizeEvent const &event);

	void onEvent(LUCID_GUI::MouseEvent const &event);

	void onButtonPress(LUCID_GUI::Button *button);

	void onCheckboxPress(LUCID_GUI::Checkbox *button);

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

	LUCID_GUI::Rectangle _rectangle;

	LUCID_GUI::Renderer _guiRender;
	LUCID_GUI::Frame *_guiConfiguring = nullptr;
	LUCID_GUI::Frame *_guiRunning = nullptr;

	LUCID_GIGL::Context _renderContext;

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
