#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/gui/Defines.h>
#include <lucid/gui/Types.h>
#include <lucid/gui/Frame.h>

LUCID_GUI_BEGIN

//	Label
//
//
class Label final : public Frame
{
public:
	Label(size_t id, ANCHOR anchor, Size const &size, ALIGNMENT align, std::string const &text, Color const &color);

	virtual ~Label() = default;

	void text(std::string const &text);

	std::string const &text() const;

	virtual void accept(Renderer *renderer) const override;

private:
	ALIGNMENT _alignment = ALIGNMENT::ALIGN_LEFT;
	std::string _text;
	Color _color;

	LUCID_PREVENT_COPY(Label);
	LUCID_PREVENT_ASSIGNMENT(Label);
};

inline void Label::text(std::string const &text)
{
	_text = text;
}

inline std::string const &Label::text() const
{
	return _text;
}

LUCID_GUI_END
