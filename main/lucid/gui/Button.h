#pragma once

#include <functional>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Frame.h>

LUCID_GUI_BEGIN

//	Button
//
//
class Button : public Frame
{
public:
	typedef std::function<void(Button *)> Callback;

	enum STATE
	{
		STATE_DISABLED = 0,
		STATE_ENABLED,
		STATE_FOCUSED,
	};

	Button(size_t id, ANCHOR anchor, int32_t width, int32_t height);

	virtual ~Button() = default;

	STATE state() const;

	void enable(bool enabled = true);

	void set(Callback const &callback);

	virtual void onEvent(TimerEvent const &event) override;

	virtual void onEvent(KeyboardEvent const &event) override;

	virtual void onEvent(MouseEvent const &event) override;

private:
	STATE _state = STATE_DISABLED;
	Callback _callback;

	LUCID_PREVENT_COPY(Button);
	LUCID_PREVENT_ASSIGNMENT(Button);
};

inline Button::STATE Button::state() const
{
	return _state;
}

inline void Button::enable(bool enabled)
{
	_state = enabled ? STATE_ENABLED : STATE_DISABLED;
}

inline void Button::set(Callback const &callback)
{
	_callback = callback;
}

LUCID_GUI_END
