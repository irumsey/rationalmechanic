#include "XprTest.h"
#include "Utility.h"
#include <lucid/xpr/xpr.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/Types.h>
#include <lucid/core/Profiler.h>
#include <memory>

namespace xpr = ::lucid::xpr;

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

	xpr::Simplify simplify;
	xpr::Repr repr;

	std::unique_ptr<xpr::Node const> function(
		xpr::div(xpr::pow(xpr::var(0),xpr::val(1)), xpr::var(0))
	);
	std::unique_ptr<xpr::Node const> simplified(simplify(function.get()));

	std::string const &repred = repr(simplified.get());

	return true;
}

void XprTest::render(float32_t time, float32_t interpolant)
{
	if (_passed)
		LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1));
	else
		LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(1, 0, 0, 1));
}
