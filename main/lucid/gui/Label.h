#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Frame.h>

LUCID_GUI_BEGIN

//	Label
//
//
class Label final : public Frame
{
public:
	Label(ANCHOR anchor, int32_t width, int32_t height, std::string const &text);

	virtual ~Label() = default;

private:
	std::string text;

	LUCID_PREVENT_COPY(Label);
	LUCID_PREVENT_ASSIGNMENT(Label);
};

LUCID_GUI_END
