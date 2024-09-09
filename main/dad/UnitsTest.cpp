#include "UnitsTest.h"
#include "Utility.h"
#include <lucid/units/System.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Profiler.h>

///
///
///

void UnitsTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting units test...");
}

bool UnitsTest::update(float64_t simTime, float64_t simTimeStep)
{
	_passed = true;

	LUCID_PROFILE_BEGIN("units test");

	LUCID_PROFILE_END();

	return true;
}

void UnitsTest::render(float32_t time, float32_t interpolant)
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1));;
}
