#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/AABB.h>
#include <lucid/gal/Types.h>
#include <lucid/gui/Defines.h>

LUCID_GUI_BEGIN

typedef LUCID_MATH::Vector<int32_t, 2> Point;
typedef LUCID_MATH::AABB<int32_t, 2> Rectangle;

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

//
//
//
struct Icon
{
#pragma pack(push)
#pragma pack(4)
	LUCID_GAL::Vector4 position;
	LUCID_GAL::Vector4 texcoord;
	LUCID_GAL::Color   color;
#pragma pack(pop)
};

LUCID_GUI_END
