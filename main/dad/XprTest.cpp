#include "XprTest.h"
#include "Utility.h"
#include <lucid/xpr/xpr.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/Types.h>
#include <lucid/core/Profiler.h>

using namespace ::lucid::xpr;

///
///
///

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

///
///
///

void XprTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting expression test...");
}

bool XprTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	Registry symbols;

    symbols.add("x", 2.0);
    symbols.add("y", 1.0);
	symbols.add("\\theta");
	symbols.add("g");

	Node const *_1 = mul(val(3.0), fn(3, { } ));

    std::string formatted = _LaTeX(_1, symbols);
	 
	delete _1;

	return true;
}

void XprTest::render(float32_t time, float32_t interpolant)
{
	if (_passed)
		LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1));
	else
		LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(1, 0, 0, 1));
}
