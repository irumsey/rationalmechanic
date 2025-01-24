#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/AABB.h>
#include <lucid/gui/Defines.h>

LUCID_GUI_BEGIN

typedef LUCID_MATH::Vector<int32_t, 2> Point;
typedef LUCID_MATH::AABB<int32_t, 2> Rectangle;

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
};

LUCID_GUI_END
