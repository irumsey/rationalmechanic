#pragma once

#include <array>
#include <functional>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Frame.h>

LUCID_GUI_BEGIN

//	Checkbox
//
//
class Checkbox : public Frame
{
public:
	typedef std::function<void(Checkbox *)> Callback;

	enum STATE
	{
		STATE_ENABLED  = 0x01,
		STATE_FOCUSED  = 0x02,
		STATE_CHECKED  = 0x04,
	};
	enum { STATE_MASK  = 0x07 };
	enum { STATE_COUNT = 0x08 };

	typedef std::array<LUCID_GAL::Vector4, STATE_COUNT> Tiles;

	Checkbox(size_t id, ANCHOR anchor, Size const &size, Tiles const &tiles, Callback const &callback);

	virtual ~Checkbox() = default;

	void setState(uint8_t state);

	uint8_t state() const;

	void setEnabled(bool enabled = true);

	bool isEnabled() const;

	void setFocused(bool focused = true);

	bool isFocused() const;

	void setChecked(bool checked = true);

	void toggleChecked();

	bool isChecked() const;

	virtual void onEvent(TimerEvent const &event) override;

	virtual void onEvent(KeyboardEvent const &event) override;

	virtual void onEvent(MouseEvent const &event) override;

	virtual void accept(Renderer *renderer) const override;

private:
	uint8_t _state = 0;	// disabled, not focused, not checked
	Tiles _tiles;

	Callback _callback;

	LUCID_PREVENT_COPY(Checkbox);
	LUCID_PREVENT_ASSIGNMENT(Checkbox);
};

inline void Checkbox::setState(uint8_t state)
{
	_state = STATE_MASK & state;
}

inline uint8_t Checkbox::state() const
{
	return _state;
}

inline void Checkbox::setEnabled(bool enabled)
{
	_state = enabled ? (STATE_ENABLED | _state) : (~STATE_ENABLED & _state);
}

inline bool Checkbox::isEnabled() const
{
	return 0 != (STATE_ENABLED & _state);
}

inline void Checkbox::setFocused(bool focused)
{
	_state = focused ? (STATE_FOCUSED | _state) : (~STATE_FOCUSED & _state);
}

inline bool Checkbox::isFocused() const
{
	return 0 != (STATE_FOCUSED & _state);
}

inline void Checkbox::setChecked(bool checked)
{
	_state = checked ? (STATE_CHECKED | _state) : (~STATE_CHECKED & _state);
}

inline void Checkbox::toggleChecked()
{
	setChecked(!isChecked());
}

inline bool Checkbox::isChecked() const
{
	return 0 != (STATE_CHECKED & _state);
}

LUCID_GUI_END
