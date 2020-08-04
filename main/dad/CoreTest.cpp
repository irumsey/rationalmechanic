#include "CoreTest.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/core/String.h>

///
///
///

namespace constants = lucid::math::constants;
namespace core = lucid::core;

namespace /* anonymous */
{

}	///	anonymous

	///
	///
	///

void CoreTest::begin(float64_t t)
{
	::log("INFO", "starting core test...");
}

bool CoreTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	return true;
}

void CoreTest::render(float32_t time, float32_t interpolant)
{
	lucid::gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	pipeline.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}
