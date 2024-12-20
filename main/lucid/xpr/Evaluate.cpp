#include "Evaluate.h"
#include "Registry.h"
#include "Node.h"

LUCID_XPR_BEGIN

float64_t Evaluate::operator()(Node const *node, Registry const &registry)
{
	symbols = &registry;

	node->apply(this);
	
	symbols = nullptr;
	return result;
}

void Evaluate::evaluate(Constant const *node)
{
	result = node->value;
}

void Evaluate::evaluate(Variable const *node)
{
	result = symbols->value_of(node->index);
}

void Evaluate::evaluate(Negate const *node)
{
	result = -rhs(node);
}

void Evaluate::evaluate(Add const *node)
{
	result = lhs(node) + rhs(node);
}

void Evaluate::evaluate(Subtract const *node)
{
	result = lhs(node) - rhs(node);
}

void Evaluate::evaluate(Multiply const *node)
{
	result = lhs(node) * rhs(node);
}

void Evaluate::evaluate(Divide const *node)
{
	result = lhs(node) / rhs(node);
}

void Evaluate::evaluate(Sine const *node)
{
	result = ::sin(rhs(node));
}

void Evaluate::evaluate(Cosine const *node)
{
	result = ::cos(rhs(node));
}

void Evaluate::evaluate(Exponential const *node)
{
	result = ::exp(rhs(node));
}

void Evaluate::evaluate(Logarithm const *node)
{
	result = ::log(rhs(node));
}

void Evaluate::evaluate(Power const *node)
{
	result = ::pow(lhs(node), rhs(node));
}

LUCID_XPR_END