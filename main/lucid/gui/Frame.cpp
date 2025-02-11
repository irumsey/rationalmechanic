#include "Frame.h"
#include <lucid/core/Profiler.h>

LUCID_GUI_BEGIN

//
//
// 

typedef Rectangle (*anchor_action_func)(Rectangle const &, int32_t, int32_t);
extern anchor_action_func computeRectangle[ANCHOR_ACTION_COUNT];

//
//
//

Frame::Frame(uint32_t id, ANCHOR anchor, Size const &size)
	: _id(id)
	, _anchor(anchor)
	, _size(size)
{
}

Frame::~Frame()
{
	delete _firstChild;
	delete _nextSibling;
}

void Frame::onEvent(SizeEvent const &event)
{
	_rectangle = computeRectangle[_anchor](event.rectangle, _size.width, _size.height);
	dispatch(SizeEvent(_rectangle));
}

//
//
//

Rectangle anchorFill(Rectangle const &frame, int32_t width, int32_t height)
{
	return frame;
}

Rectangle anchorCenter(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = (frame.max.x + frame.min.x - width) >> 1;
	int32_t xmax = (frame.max.x + frame.min.x + width) >> 1;
	
	int32_t ymin = (frame.max.y + frame.min.y - height) >> 1;
	int32_t ymax = (frame.max.y + frame.min.y + height) >> 1;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorNorth(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = (frame.max.x + frame.min.x - width) >> 1;
	int32_t xmax = (frame.max.x + frame.min.x + width) >> 1;

	int32_t ymin = frame.max.y - height;
	int32_t ymax = frame.max.y;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorNorthEast(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = frame.max.x -  width;
	int32_t ymin = frame.max.y - height;

	return Rectangle(Point(xmin, ymin), frame.max);
}

Rectangle anchorEast(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = frame.max.x - width;
	int32_t xmax = frame.max.x;

	int32_t ymin = (frame.max.y + frame.min.y - height) >> 1;
	int32_t ymax = (frame.max.y + frame.min.y + height) >> 1;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorSouthEast(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = frame.max.x - width;
	int32_t xmax = frame.max.x;

	int32_t ymin = frame.min.y;
	int32_t ymax = frame.min.y + height;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorSouth(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = (frame.max.x + frame.min.x - width) >> 1;
	int32_t xmax = (frame.max.x + frame.min.x + width) >> 1;

	int32_t ymin = frame.min.y;
	int32_t ymax = frame.min.y + height;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorSouthWest(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmax = frame.min.x +  width;
	int32_t ymax = frame.min.y + height;

	return Rectangle(frame.min, Point(xmax, ymax));
}

Rectangle anchorWest(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = frame.min.x;
	int32_t xmax = frame.min.x + width;

	int32_t ymin = (frame.max.y + frame.min.y - height) >> 1;
	int32_t ymax = (frame.max.y + frame.min.y + height) >> 1;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

Rectangle anchorNorthWest(Rectangle const &frame, int32_t width, int32_t height)
{
	int32_t xmin = frame.min.x;
	int32_t xmax = frame.min.x + width;

	int32_t ymin = frame.max.y - height;
	int32_t ymax = frame.max.y;

	return Rectangle(Point(xmin, ymin), Point(xmax, ymax));
}

anchor_action_func computeRectangle[ANCHOR_ACTION_COUNT] =
{
	anchorFill,
	anchorCenter,
	anchorNorth,
	anchorNorthEast,
	anchorEast,
	anchorSouthEast,
	anchorSouth,
	anchorSouthWest,
	anchorWest,
	anchorNorthWest,
};

LUCID_GUI_END
