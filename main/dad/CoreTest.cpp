#include "CoreTest.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Logger.h>
#include <lucid/core/String.h>

///
///
///

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

///
///
///

void CoreTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting core test...");
}

bool CoreTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	// TBD: tests here...

	return true;
}

void CoreTest::render(float32_t time, float32_t interpolant)
{
	LUCID_GAL_PIPELINE.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
