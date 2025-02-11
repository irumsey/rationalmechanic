#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Defines.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Context.h>
#include <lucid/gui/Types.h>
#include <lucid/gui/Events.h>
#include <lucid/gui/Renderer.h>
#include <rnd/Utility.h>
#include <rnd/DummySimulator.h>

LUCID_GAL_BEGIN

class RenderTarget2D;
class TargetReader2D;
class Parameter;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Mesh;

LUCID_GIGL_END

LUCID_GUI_BEGIN

class Frame;
class Label;
class Button;
class Checkbox;

LUCID_GUI_END

class State;

/// Session
/// 
/// This is where all the top level application data resides.
/// The intent is to use the state pattern and define
/// Session as the noun and State as the action.
/// 
/// See: State
/// 
/// Note: in the implementation of its members, Session defers
/// all action to the current state.  However, a few exceptions
/// are allowed so the each state need not preprocess basic data.
/// For example, processing screen and mouse coordinates to
/// make sure they are consistent.
class Session
{
public:
	Session();

	virtual ~Session() = default;

	void initialize(LUCID_GUI::Rectangle const &rectangle);

	void shutdown();

	void onEvent(LUCID_GUI::TimerEvent const &event);

	void onEvent(LUCID_GUI::SizeEvent const &event);

	void onEvent(LUCID_GUI::MouseEvent const &event);

	void onButtonPress(LUCID_GUI::Button *button);

	void onCheckboxPress(LUCID_GUI::Checkbox *button);

	void render(float64_t time);

private:
	friend class State;
	friend class Stopped;
	friend class Starting;
	friend class Stopping;
	friend class Configuring;
	friend class Running;

	struct BlurParameters
	{
		LUCID_GAL::Parameter const *texelOffset = nullptr;
		LUCID_GAL::Parameter const *theSource = nullptr;
	};

	struct PostParameters
	{
		LUCID_GAL::Parameter const *colorTarget = nullptr;
		LUCID_GAL::Parameter const *glowTarget = nullptr;
	};

	State const *_state = nullptr;
	void changeState(State const *state);

	LUCID_GUI::Rectangle _rectangle;

	LUCID_GAL::RenderTarget2D *_selectTarget = nullptr;
	LUCID_GAL::RenderTarget2D *_colorTarget = nullptr;
	LUCID_GAL::RenderTarget2D *_glowTarget = nullptr;
	LUCID_GAL::RenderTarget2D *_depthTarget = nullptr;

	LUCID_GAL::TargetReader2D *_selectReader = nullptr;

	LUCID_GAL::RenderTarget2D *_blurTarget = nullptr;

	LUCID_GIGL::Mesh *_clear = nullptr;

	LUCID_GIGL::Mesh *_blur = nullptr;
	BlurParameters _blurParameters;

	LUCID_GIGL::Mesh *_post = nullptr;
	PostParameters _postParameters;

	LUCID_GUI::Renderer _guiRender;
	LUCID_GUI::Frame *_guiConfiguring = nullptr;
	LUCID_GUI::Frame *_guiRunning = nullptr;
	LUCID_GUI::Label *_guiTimeDisplay = nullptr;
	LUCID_GUI::Label *_guiCurrentSelection = nullptr;

	LUCID_GIGL::Context _renderContext;

	DummySimulator _simulator;

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
