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
	virtual ~Frame();

	Frame const *firstChild() const;

	Frame const *nextSibling() const;

	uint32_t id() const;

	ANCHOR anchor() const;

	Size const &size() const;

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
	Frame(uint32_t id, ANCHOR anchor, Size const &size);

private:
	Frame *_firstChild = nullptr;
	Frame *_nextSibling = nullptr;

	uint32_t _id = 0;
	ANCHOR _anchor = ANCHOR_FILL;
	Size _size;

	Rectangle _rectangle;

	bool _hasFocus = false;

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

inline uint32_t Frame::id() const
{
	return _id;
}

inline ANCHOR Frame::anchor() const
{
	return _anchor;
}

inline Size const &Frame::size() const
{
	return _size;
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
	if (_hasFocus)
		dispatch(event);
}

inline void Frame::onEvent(MouseEvent const &event)
{
	// children will usually share edges with its parent.
	// if a child had focus but the parent does not, the
	// child needs to know.
	bool hadFocus = _hasFocus;
	_hasFocus = isHit(event.position);

	if (_hasFocus || hadFocus)
		dispatch(event);
}

template<typename E> inline void Frame::dispatch(E const &event)
{
	for (Frame *child = _firstChild; nullptr != child; child = child->_nextSibling)
		child->onEvent(event);
}

LUCID_GUI_END
