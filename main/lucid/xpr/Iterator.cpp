#include "Iterator.h"
#include "Node.h"

LUCID_XPR_BEGIN

Iterator::Iterator(Node const *node)
{
	set(node);
}

Iterator &Iterator::operator++()
{
	current->apply(this);
	return *this;
}

Iterator Iterator::operator++(int)
{
	Iterator temp = *this;
	current->apply(this);
	return temp;
}

void Iterator::evaluate(Constant const *node)
{
	evaluateLeaf(node);
}

void Iterator::evaluate(Variable const *node)
{
	evaluateLeaf(node);
}

void Iterator::evaluate(Function const *node)
{
	evaluateLeaf(node);
}

void Iterator::evaluate(Derivative const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Negate const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Add const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Subtract const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Multiply const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Divide const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Sine const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Cosine const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Exponential const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluate(Logarithm const *node)
{
	evaluateOperation(node);
}

void Iterator::evaluateLeaf(Node const *node)
{
	pop();
}

void Iterator::evaluateOperation(UnaryOperation const *node)
{
	current = _rhs(node);
}

void Iterator::evaluateOperation(BinaryOperation const *node)
{
	current = _lhs(node);
	stack.push_back(_rhs(node));
}

LUCID_XPR_END
