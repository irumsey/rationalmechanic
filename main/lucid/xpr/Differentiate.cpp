#include "Differentiate.h"
#include "Node.h"
#include <cassert>

LUCID_XPR_BEGIN

Node const *Differentiate::operator()(Node const *node, std::string const &symbol_)
{
	symbol = symbol_;
	node->apply(this);
	return result;
}

void Differentiate::evaluate(Constant const *node)
{
	result = val(0.0);
}

void Differentiate::evaluate(Variable const *node)
{
	result = (symbol == node->symbol) ? val(1.0) : val(0.0);
}

void Differentiate::evaluate(Negate const *node)
{
	result = neg(dv(node));
}

void Differentiate::evaluate(NaturalLogarithm const *node)
{
	result = div(dv(node), v(node));
}

void Differentiate::evaluate(Sine const *node)
{
	result = mul(dv(node), cos(v(node)));
}

void Differentiate::evaluate(Cosine const *node)
{
	result = neg(mul(dv(node), sin(v(node))));
}

void Differentiate::evaluate(Add const *node)
{
	result = add(du(node), dv(node));
}

void Differentiate::evaluate(Subtract const *node)
{
	result = sub(du(node), dv(node));
}

void Differentiate::evaluate(Multiply const *node)
{
	result = add(mul(u(node), dv(node)), mul(v(node), du(node)));
}

void Differentiate::evaluate(Divide const *node)
{
	result = div(sub(mul(v(node), du(node)), mul(u(node), dv(node))), pow(v(node), val(2.0)));
}

void Differentiate::evaluate(Power const *node)
{
	result = mul(pow(u(node), v(node)), add(mul(du(node), div(v(node), u(node))), mul(dv(node), ln(u(node)))));
}

LUCID_XPR_END