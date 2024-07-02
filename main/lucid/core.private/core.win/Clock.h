#pragma once

#ifndef CORE_PRIVATE
#	error core layer violation
#endif

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Clock.h>
#include <lucid/core.private/core.win/Defines.h>

LUCID_CORE_WIN_BEGIN

///
///
///
class Clock : public LUCID_CORE::Clock
{
public:
	Clock();

	virtual ~Clock();

	virtual float64_t time() const override;

	virtual void reset() override;

private:
	float64_t _period = 0;
	int64_t _startCount = 0;

	LUCID_PREVENT_COPY(Clock);
	LUCID_PREVENT_ASSIGNMENT(Clock);
};

LUCID_CORE_WIN_END