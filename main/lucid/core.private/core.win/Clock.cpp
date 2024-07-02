#include "Clock.h"
#include <lucid/core/Error.h>
#include <lucid/core/Types.h>
#include <windows.h>

LUCID_CORE_BEGIN

Clock *Clock::create()
{
	return new win::Clock();
}

LUCID_CORE_END

LUCID_CORE_WIN_BEGIN

Clock::Clock()
{
	LARGE_INTEGER frequency;
	LUCID_VALIDATE(::QueryPerformanceFrequency(&frequency), "unable to determine clock frequency");

	_period = 1.0 / frequency.QuadPart;

	reset();
}

Clock::~Clock()
{
}

float64_t Clock::time() const
{
	int64_t count;
	LUCID_VALIDATE(::QueryPerformanceCounter((LARGE_INTEGER*)&count), "unable to determine current count");

	return _period * (float64_t)(count - _startCount);
}

void Clock::reset()
{
	LUCID_VALIDATE(::QueryPerformanceCounter((LARGE_INTEGER*)&_startCount), "unable to determine start count");
}

LUCID_CORE_WIN_END