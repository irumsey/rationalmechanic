#include "Clone.h"
#include "Node.h"

LUCID_XPR_BEGIN

Node const *Clone::operator()(Node const *node)
{
	node->apply(this);
	return result;
}

void Clone::evaluate(Constant const *node)
{
	result = val(node->value);
}

void Clone::evaluate(Variable const *node)
{
	result = var(node->index);
}

void Clone::evaluate(Negate const *node)
{
	result = neg(rhs(node));
}

void Clone::evaluate(Add const *node)
{
	result = add(lhs(node), rhs(node));
}

void Clone::evaluate(Subtract const *node)
{
	result = sub(lhs(node), rhs(node));
}

void Clone::evaluate(Multiply const *node)
{
	result = mul(lhs(node), rhs(node));
}

void Clone::evaluate(Divide const *node)
{
	result = div(lhs(node), rhs(node));
}

void Clone::evaluate(Sine const *node)
{
	result = sin(rhs(node));
}

void Clone::evaluate(Cosine const *node)
{
	result = cos(rhs(node));
}

void Clone::evaluate(Exponential const *node)
{
	result = exp(rhs(node));
}

void Clone::evaluate(Logarithm const *node)
{
	result = log(rhs(node));
}

void Clone::evaluate(Power const *node)
{
	result = pow(lhs(node), rhs(node));
}

LUCID_XPR_END