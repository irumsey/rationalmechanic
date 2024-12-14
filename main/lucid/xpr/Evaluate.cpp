#include "Evaluate.h"
#include "Node.h"
#include <cassert>

LUCID_XPR_BEGIN

float64_t Evaluate::operator()(Node const *node, Symbols const &symbols_)
{
	///	just make a copy of "symbols"
	symbols = symbols_;
	node->apply(this);
	return result;
}

void Evaluate::evaluate(Any const *)
{
	// TBD: for now, treat a wild card node as a NOP...
}

void Evaluate::evaluate(Constant const *node)
{
	result = node->value;
}

void Evaluate::evaluate(Variable const *node)
{
	auto iter = symbols.find(node->symbol);
	assert(iter != symbols.end());

	result = iter->second;
}

void Evaluate::evaluate(Negate const *node)
{
	result = -rhs(node);
}

void Evaluate::evaluate(NaturalLogarithm const *node)
{
	result = ::log(rhs(node));
}

void Evaluate::evaluate(Sine const *node)
{
	result = ::sin(rhs(node));
}

void Evaluate::evaluate(Cosine const *node)
{
	result = ::cos(rhs(node));
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

void Evaluate::evaluate(Power const *node)
{
	result = ::pow(lhs(node), rhs(node));
}

LUCID_XPR_END