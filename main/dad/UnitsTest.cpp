#include "UnitsTest.h"
#include "Utility.h"
#include <lucid/units/Units.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Vector.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Profiler.h>

///
///
///

namespace  core = lucid::core;
namespace units = lucid::units;
namespace    si = lucid::units::si;
namespace  math = lucid::math;

namespace /* anonymous */
{

}	///	anonymous

///
///
///

void UnitsTest::begin(float64_t t)
{
	::log("INFO", "starting units test...");
}

bool UnitsTest::update(float64_t simTime, float64_t simTimeStep)
{
	_passed = true;

	LUCID_PROFILE_BEGIN("units test");

	si::pure const half = 0.5f;

	si::acceleration a = -9.8f;
	si::velocity v0 = 100.f;
	si::meter d0 = 10.f;

	for (size_t i = 0; i < 1000; ++i)
	{
		si::second t = 0.01f * i;
		si::meter d = half * a * t * t + t * v0 + d0;
	}

	LUCID_PROFILE_END();

	return true;
}

void UnitsTest::render(float32_t time, float32_t interpolant)
{
	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
