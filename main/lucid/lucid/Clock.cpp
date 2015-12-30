#include "Clock.h"
#include <lucid/core/Clock.h>

namespace lucid {

	Clock::Clock()
	{
		_internal = ::lucid::core::Clock::create();
	}

	Clock::~Clock()
	{
		this->!Clock();
	}

	Clock::!Clock()
	{
		delete _internal;
	}

	float64_t Clock::time::get()
	{
		return _internal->time();
	}

	void Clock::reset()
	{
		_internal->reset();
	}

}	///  lucid
