#include "Clock.h"
#include <lucid/core/Clock.h>

namespace Lucid {
namespace Core {

Clock::Clock()
{
	_internal = LUCID_CORE::Clock::create();
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

}
}
