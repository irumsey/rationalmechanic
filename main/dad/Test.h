#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

///
///
///
class Test
{
public:
	virtual ~Test() = default;

	virtual bool passed() const = 0;

	virtual void begin(float64_t t) = 0;

	virtual bool update(float64_t t, float64_t dt) = 0;

	virtual void render(float32_t time, float32_t interpolant) = 0;

protected:
	Test() = default;

	LUCID_PREVENT_COPY(Test);
	LUCID_PREVENT_ASSIGNMENT(Test);
};
