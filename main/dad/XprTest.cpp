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

	Differentiate differentiate;
	Simplify simplify;
    Registry symbols;
    LaTeX format;

    symbols.add("x", 2.0);
    symbols.add("y", 1.0);
	symbols.add("g");

	Node const *_1 = mul(val(3), div(fn(2, { 0, 1, }), var(symbols.index_of("x"))));
	Node const *_2 = differentiate(_1, symbols.index_of("x"));
	Node const *_3 = simplify(_2, symbols);

    std::string const &formatted = format(_3, symbols);
	 
	delete _3;
	delete _2;
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
