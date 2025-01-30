#pragma once

#include <array>
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
		STATE_PRESSED,
	};
	enum { STATE_COUNT = STATE_PRESSED + 1 };

	typedef std::array<LUCID_GAL::Vector4, STATE_COUNT> Tiles;

	Button(size_t id, ANCHOR anchor, int32_t width, int32_t height, Callback const &callback, Tiles const &tiles);

	virtual ~Button() = default;

	STATE state() const;

	void enable(bool enabled = true);

	virtual void onEvent(TimerEvent const &event) override;

	virtual void onEvent(KeyboardEvent const &event) override;

	virtual void onEvent(MouseEvent const &event) override;

	virtual void accept(Renderer *renderer) const override;

private:
	STATE _state = STATE_DISABLED;
	Tiles _tiles;

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

LUCID_GUI_END
