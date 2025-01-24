#pragma once

#include <cassert>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>

LUCID_GUI_BEGIN

//	Frame
//
//
class Frame 
{
public:
	enum ANCHOR
	{
		ANCHOR_FILL = 0,
		ANCHOR_CENTER,
		ANCHOR_NORTH,
		ANCHOR_NORTH_EAST,
		ANCHOR_EAST,
		ANCHOR_SOUTH_EAST,
		ANCHOR_SOUTH,
		ANCHOR_SOUTH_WEST,
		ANCHOR_WEST,
		ANCHOR_NORTH_WEST,
	};
	enum { ANCHOR_ACTION_COUNT = ANCHOR_NORTH_WEST + 1 };

	virtual ~Frame();

	virtual void addChild(Frame *frame);

	virtual void onEvent(SizeEvent const &event);

	virtual void onEvent(TimerEvent const &event);

	virtual void onEvent(KeyboardEvent const &event);

	virtual void onEvent(MouseEvent const &event);

protected:
	Frame(ANCHOR anchor, int32_t width, int32_t height);

private:
	Frame *firstChild = nullptr;
	Frame *nextSibling = nullptr;

	ANCHOR anchor = ANCHOR_FILL;
	int32_t width = 0;
	int32_t height = 0;

	Rectangle rectangle;

	template<typename E> void dispatch(E const &event);

	LUCID_PREVENT_COPY(Frame);
	LUCID_PREVENT_ASSIGNMENT(Frame);
};

inline void Frame::addChild(Frame *frame)
{
	assert(nullptr != frame);

	frame->nextSibling = firstChild;
	firstChild = frame;
}

inline void Frame::onEvent(TimerEvent const &event)
{
	dispatch(event);
}

inline void Frame::onEvent(KeyboardEvent const &event)
{
	dispatch(event);
}

inline void Frame::onEvent(MouseEvent const &event)
{
	dispatch(event);
}

template<typename E> inline void Frame::dispatch(E const &event)
{
	for (Frame *child = firstChild; nullptr != child; child = child->nextSibling)
		child->onEvent(event);
}

LUCID_GUI_END
