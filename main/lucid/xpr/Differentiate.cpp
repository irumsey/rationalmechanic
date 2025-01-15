#include "Differentiate.h"
#include "Node.h"

LUCID_XPR_BEGIN

Node const *Differentiate::operator()(Node const *node, uint64_t wrt)
{
	index = wrt;
	node->apply(this);
	return result;
}

void Differentiate::evaluate(Constant const *node)
{
	result = val(0.0);
}

void Differentiate::evaluate(Variable const *node)
{
	result = (index == node->index) ? val(1.0) : val(0.0);
}

void Differentiate::evaluate(Function const *node)
{
	result = ddx(clone(node), index);
}

void Differentiate::evaluate(Derivative const *node)
{
	result = ddx(clone(node), index);
}

void Differentiate::evaluate(Negate const *node)
{
	result = neg(dv(node));
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

void Differentiate::evaluate(Sine const *node)
{
	result = mul(dv(node), cos(v(node)));
}

void Differentiate::evaluate(Cosine const *node)
{
	result = neg(mul(dv(node), sin(v(node))));
}

void Differentiate::evaluate(Exponential const *node)
{
	result = mul(clone(node), dv(node)); 
}

void Differentiate::evaluate(Logarithm const *node)
{
	result = div(dv(node), v(node));
}

LUCID_XPR_END