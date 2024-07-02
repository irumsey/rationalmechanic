#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class RenderTarget2D;


///	TargetReader2D
///
///
class TargetReader2D
{
public:
	virtual ~TargetReader2D() = 0 {}

	virtual int32_t width() const = 0;

	virtual int32_t height() const = 0;

	virtual uint8_t const *read() const = 0;

	static TargetReader2D *create(RenderTarget2D const *target, int32_t width, int32_t height);

protected:
	TargetReader2D() {}

	LUCID_PREVENT_COPY(TargetReader2D);
	LUCID_PREVENT_ASSIGNMENT(TargetReader2D);
};

LUCID_GAL_END