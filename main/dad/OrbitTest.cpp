#include "OrbitTest.h"
#include "Utility.h"
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Elements.h>
#include <lucid/orbit/Properties.h>
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

	orbit::Properties properties;
	ephemeris.lookup(properties, "Earth");

	orbit::Elements elements;
	size_t center = ephemeris.lookup(elements, "Earth", 3524593.5f);

	orbit::vector3_t position;
	orbit::vector3_t velocity;
	ephemeris.compute(position, velocity, "Earth", 2451544.5f);

	/// test {
	///	convert to au and au/day to verify against Horizons
	position = position / 1.495978e11f;
	velocity = 86400.f * velocity / 1.495978e11f;
	/// } test
	LUCID_PROFILE_END();

	return true;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
