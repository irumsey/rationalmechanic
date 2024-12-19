#include "Hasher.h"
#include "Node.h"

LUCID_XPR_BEGIN

Token Hasher::hash(Node const *node)
{
	token = Token();
	node->apply(this);
	return token;
}

void Hasher::evaluate(Constant const *node)
{
	token.tid = TYPE<Constant>::ID();
	token.value = node->value;
}

void Hasher::evaluate(Variable const *node)
{
	token.tid = TYPE<Variable>::ID();
	token.index = node->index;
}

void Hasher::evaluate(Negate const *node)
{
	token.tid = TYPE<Negate>::ID();
}

void Hasher::evaluate(Add const *node)
{
	token.tid = TYPE<Add>::ID();
}

void Hasher::evaluate(Subtract const *node)
{
	token.tid = TYPE<Subtract>::ID();
}

void Hasher::evaluate(Multiply const *node)
{
	token.tid = TYPE<Multiply>::ID();
}

void Hasher::evaluate(Divide const *node)
{
	token.tid = TYPE<Divide>::ID();
}

void Hasher::evaluate(Sine const *node)
{
	token.tid = TYPE<Sine>::ID();
}

void Hasher::evaluate(Cosine const *node)
{
	token.tid = TYPE<Cosine>::ID();
}

void Hasher::evaluate(Exponential const *node)
{
	token.tid = TYPE<Exponential>::ID();
}

void Hasher::evaluate(Logarithm const *node)
{
	token.tid = TYPE<Logarithm>::ID();
}

void Hasher::evaluate(Power const *node)
{
	token.tid = TYPE<Power>::ID();
}

LUCID_XPR_END
