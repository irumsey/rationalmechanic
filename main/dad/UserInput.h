#pragma once

#include <lucid/core/Types.h>

///	MouseEvent
///
///
struct MouseEvent
{
	int32_t x = 0;
	int32_t y = 0;
	int16_t z = 0;

	bool btnDownLeft = false;
	bool btnDownMiddle = false;
	bool btnDownRight = false;
};
