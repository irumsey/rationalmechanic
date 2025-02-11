#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Math.h>
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

	template<typename T> T sample(int32_t x, int32_t y) const;

	static TargetReader2D *create(RenderTarget2D const *target, int32_t width, int32_t height);

protected:
	TargetReader2D() {}

	LUCID_PREVENT_COPY(TargetReader2D);
	LUCID_PREVENT_ASSIGNMENT(TargetReader2D);
};

template<typename T> inline T TargetReader2D::sample(int32_t x, int32_t y) const
{
	x = LUCID_MATH::clamp(x, 0,  width() - 1);
	y = LUCID_MATH::clamp(y, 0, height() - 1);

	T const *data = reinterpret_cast<T const *>(read());

	return data[y * width() + x];
}

LUCID_GAL_END