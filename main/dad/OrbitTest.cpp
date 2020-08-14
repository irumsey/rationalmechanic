#include "OrbitTest.h"
#include "Utility.h"
#include <lucid/orbit/Ephemeris.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/String.h>

///
///
///

namespace constants = lucid::math::constants;
namespace core = lucid::core;
namespace math = lucid::math;
namespace orbit = lucid::orbit;

namespace /* anonymous */
{

}	///	anonymous

	///
	///
	///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");
}

bool OrbitTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	LUCID_PROFILE_BEGIN("ephemeris test");
	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();
	ephemeris.initialize("content/j2000.ephemeris");
	orbit::Elements elements = ephemeris.lookup(2451545.f, "Earth");
	LUCID_PROFILE_END();

	return true;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
