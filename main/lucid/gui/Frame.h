#pragma once

#include <cassert>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>
#include <lucid/gui/Events.h>

LUCID_GUI_BEGIN

class Renderer;

//	Frame
//
//
class Frame : public EventHandler
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

	Frame const *firstChild() const;

	Frame const *nextSibling() const;

	size_t id() const;

	ANCHOR anchor() const;

	int32_t width() const;

	int32_t height() const;

	Rectangle const &rectangle() const;

	bool isHit(LUCID_GUI::Point const &position) const;

	bool notHit(LUCID_GUI::Point const &position) const;

	virtual void addChild(Frame *frame);

	virtual void onEvent(SizeEvent const &event) override;

	virtual void onEvent(TimerEvent const &event) override;

	virtual void onEvent(KeyboardEvent const &event) override;

	virtual void onEvent(MouseEvent const &event) override;

	virtual void accept(Renderer *renderer) const = 0;

protected:
	Frame(size_t id, ANCHOR anchor, int32_t width, int32_t height);

private:
	Frame *_firstChild = nullptr;
	Frame *_nextSibling = nullptr;

	size_t _id = -1;
	ANCHOR _anchor = ANCHOR_FILL;
	int32_t _width = 0;
	int32_t _height = 0;

	Rectangle _rectangle;

	template<typename E> void dispatch(E const &event);

	LUCID_PREVENT_COPY(Frame);
	LUCID_PREVENT_ASSIGNMENT(Frame);
};

inline Frame const *Frame::firstChild() const
{
	return _firstChild;
}

inline Frame const *Frame::nextSibling() const
{
	return _nextSibling;
}

inline size_t Frame::id() const
{
	return _id;
}

inline Frame::ANCHOR Frame::anchor() const
{
	return _anchor;
}

inline int32_t Frame::width() const
{
	return _width;
}

inline int32_t Frame::height() const
{
	return _height;
}

inline Rectangle const &Frame::rectangle() const
{
	return _rectangle;
}

inline bool Frame::isHit(LUCID_GUI::Point const &position) const
{
	return LUCID_MATH::contains(_rectangle, position);
}

inline bool Frame::notHit(LUCID_GUI::Point const &position) const
{
	return !isHit(position);
}

inline void Frame::addChild(Frame *frame)
{
	assert(nullptr != frame);

	frame->_nextSibling = _firstChild;
	_firstChild = frame;
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
	if (isHit(event.position))
		dispatch(event);
}

template<typename E> inline void Frame::dispatch(E const &event)
{
	for (Frame *child = _firstChild; nullptr != child; child = child->_nextSibling)
		child->onEvent(event);
}

LUCID_GUI_END
