#include <rnd/xpr/Simplify.h>
#include <rnd/xpr/Differentiate.h>
#include <rnd/xpr/Evaluate.h>
#include <rnd/xpr/Repr.h>
#include <rnd/xpr/Clone.h>
#include <rnd/xpr/Node.h>
#include <rnd/xpr/Symbols.h>
#include <iostream>

using namespace rnd::xpr;

int main()
{
	double const e = 2.718281828;

	Clone clone;
	Repr repr(std::cout);
	Evaluate evaluate;
	Differentiate differentiate;
	Simplify simplify;

	Symbols symbols;
	symbols["x"] = 7.0;

	{
		Node const *expr = add(mul(val(2.0), var("x")), val(1.0));
		Node const *copy = clone(expr);
		Node const *ddx = differentiate(copy, "x");
		Node const *simplified = simplify(ddx);

		repr(expr);
		std::cout << " -> " << evaluate(expr, symbols) << std::endl;

		repr(copy);
		std::cout << " -> " << evaluate(copy, symbols) << std::endl;

		repr(ddx);
		std::cout << " -> " << evaluate(ddx, symbols) << std::endl;

		repr(simplified);
		std::cout << " -> " << evaluate(simplified, symbols) << std::endl;

		delete simplified;
		delete ddx;
		delete copy;
		delete expr;
	}

	{
		Node const *expr = sub(mul(val(2.0), ln(mul(val(3.0), var("x")))), pow(val(e), var("x")));
		Node const *ddx = differentiate(expr, "x");
		Node const *simplified = simplify(ddx);

		repr(expr);
		std::cout << " -> " << evaluate(expr, symbols) << std::endl;

		repr(ddx);
		std::cout << " -> " << evaluate(ddx, symbols) << std::endl;

		repr(simplified);
		std::cout << " -> " << evaluate(simplified, symbols) << std::endl;

		delete simplified;
		delete ddx;
		delete expr;
	}

	{
		Node const *expr = div(val(2.0), var("x"));
		Node const *ddx = differentiate(expr, "x");
		Node const *simplified = simplify(ddx);

		repr(expr);
		std::cout << " -> " << evaluate(expr, symbols) << std::endl;

		repr(ddx);
		std::cout << " -> " << evaluate(ddx, symbols) << std::endl;

		repr(simplified);
		std::cout << " -> " << evaluate(simplified, symbols) << std::endl;

		delete simplified;
		delete ddx;
		delete expr;
	}

	return 0;
}
