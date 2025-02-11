#pragma once

#include <lucid/gui/Defines.h>
#include <rnd/Utility.h>

LUCID_GUI_BEGIN

struct TimerEvent;
struct SizeEvent;
struct MouseEvent;

class Button;
class Checkbox;

LUCID_GUI_END

class Session;

///	State
///
/// 
class State
{
public:
	virtual ~State() = default;

	virtual void onEnter(Session *session) const = 0;

	virtual void onLeave(Session *session) const = 0;

	virtual void onEvent(Session *session, LUCID_GUI::TimerEvent const &event) const { /* nop */ }

	virtual void onEvent(Session *session, LUCID_GUI::SizeEvent const &event) const;

	virtual void onEvent(Session *session, LUCID_GUI::MouseEvent const &event) const { /* nop */ }

	virtual void onButtonPress(Session *session, LUCID_GUI::Button *button) const { /* nop */ }

	virtual void onCheckboxPress(Session *session, LUCID_GUI::Checkbox *button) const { /* nop */ }

	virtual void render(Session *session, float64_t time) const { /* nop */ }

protected:
	State() = default;

	void preRender(Session *session, float64_t time) const;

	void blurGlowTarget(Session *session) const;

	void postRender(Session *session) const;

};

///	Stopped
///
///	default, inert, state
class Stopped final : public State
{
public:
	Stopped() = default;

	virtual ~Stopped() = default;

	virtual void onEnter(Session *session) const override;

	virtual void onLeave(Session *session) const override;

	static Stopped const *instance();

};

///	Starting
///
///	upon enter, this state will initialize the session
class Starting final : public State
{
public:
	Starting() = default;

	virtual ~Starting() = default;

	virtual void onEnter(Session *session) const override;

	virtual void onLeave(Session *session) const override;

	static Starting const *instance();

};

///	Stopping
///
///	upon enter, this state will clean up the session
class Stopping final : public State
{
public:
	Stopping() = default;

	virtual ~Stopping() = default;

	virtual void onEnter(Session *session) const override;

	virtual void onLeave(Session *session) const override;

	static Stopping const *instance();

};

///	Configuring
/// 
/// this is the "main menu" state where the user sets up,
/// or configures, the simulation
class Configuring final : public State
{
public:
	Configuring() = default;

	virtual ~Configuring() = default;

	virtual void onEnter(Session *session) const override;

	virtual void onLeave(Session *session) const override;

	virtual void onEvent(Session *session, LUCID_GUI::TimerEvent const &event) const override;

	virtual void onEvent(Session *session, LUCID_GUI::MouseEvent const &event) const override;

	virtual void onButtonPress(Session *session, LUCID_GUI::Button *button) const override;

	virtual void render(Session *session, float64_t time) const override;

	static Configuring const *instance();
};

///	Running
///
/// upon enter, this state will begin simulating
class Running final : public State
{
public:
	Running() = default;

	virtual ~Running() = default;

	virtual void onEnter(Session *session) const override;

	virtual void onLeave(Session *session) const override;

	virtual void onEvent(Session *session, LUCID_GUI::TimerEvent const &event) const override;

	virtual void onEvent(Session *session, LUCID_GUI::MouseEvent const &event) const override;

	virtual void onButtonPress(Session *session, LUCID_GUI::Button *button) const override;

	virtual void onCheckboxPress(Session *session, LUCID_GUI::Checkbox *button) const override;

	virtual void render(Session *session, float64_t time) const override;

	static Running const *instance();

};
