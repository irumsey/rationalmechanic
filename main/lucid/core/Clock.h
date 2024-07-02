#pragma once

///
///
///

#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

///	Clock
///
///	free running clock.
class Clock
{
public:
	virtual ~Clock() = default;

	virtual float64_t time() const = 0;

	virtual void reset() = 0;

	static Clock *create();

protected:
	Clock() = default;

	LUCID_PREVENT_COPY(Clock);
	LUCID_PREVENT_ASSIGNMENT(Clock);
};

LUCID_CORE_END