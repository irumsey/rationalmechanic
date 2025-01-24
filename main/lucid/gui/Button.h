#pragma once

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
	Button(ANCHOR anchor, int32_t width, int32_t height);

	virtual ~Button() = default;

	LUCID_PREVENT_COPY(Button);
	LUCID_PREVENT_ASSIGNMENT(Button);
};

LUCID_GUI_END
