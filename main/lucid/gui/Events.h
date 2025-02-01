#pragma once

#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>

LUCID_GUI_BEGIN

//
//
//
struct SizeEvent
{
	Rectangle rectangle;

	SizeEvent(Rectangle const &rectangle)
		: rectangle(rectangle)
	{
	}
};

//
//
//
struct TimerEvent
{
	float64_t t = 0.0;
	float32_t dt = 0.f;

	TimerEvent() = default;

	TimerEvent(float64_t t, float32_t dt)
		: t(t)
		, dt(dt)
	{
	}

};

//
//
//
struct KeyboardEvent
{
};

//
//
//
struct MouseEvent
{
	enum KIND
	{
		KIND_NONE,
		KIND_BUTTON_DOWN,
		KIND_BUTTON_UP,
		KIND_WHEEL_MOVE,
		KIND_POINTER_MOVE,
	};

	enum BUTTON
	{
		BUTTON_NONE = 0,
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT,
	};

	KIND kind = KIND_NONE;
	BUTTON button = BUTTON_NONE;
	int32_t delta = 0;
	Point position = { 0, 0, };

	MouseEvent() = default;

	MouseEvent(KIND kind, BUTTON button, int32_t delta, Point const &position)
		: kind(kind)
		, button(button)
		, delta(delta)
		, position(position)
	{
	}

};

//
//
//
class EventHandler
{
public:
	virtual ~EventHandler() = default;

	virtual void onEvent(SizeEvent const &event) = 0;

	virtual void onEvent(TimerEvent const &event) = 0;

	virtual void onEvent(KeyboardEvent const &event) = 0;

	virtual void onEvent(MouseEvent const &event) = 0;

protected:
	EventHandler() = default;

};

LUCID_GUI_END