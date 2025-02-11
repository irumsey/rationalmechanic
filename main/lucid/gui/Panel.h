#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Frame.h>

LUCID_GUI_BEGIN

//	Panel
//
//
class Panel final : public Frame
{
public:
	Panel(uint32_t id, ANCHOR anchor, Size const &size);

	virtual ~Panel() = default;

	virtual void accept(Renderer *renderer) const override;

	LUCID_PREVENT_COPY(Panel);
	LUCID_PREVENT_ASSIGNMENT(Panel);
};

LUCID_GUI_END