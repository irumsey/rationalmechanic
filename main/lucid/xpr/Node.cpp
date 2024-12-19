#include "Node.h"
#include "Algorithm.h"
#include <cassert>

LUCID_XPR_BEGIN

///
///
///

#if defined(_DEBUG)
	size_t Node::instances = 0;
#endif

///
///
///

Constant::Constant(float64_t const &value)
	: value(value)
{
}

void Constant::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Variable::Variable(uint64_t index)
	: index(index)
{
}

void Variable::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

UnaryOperation::UnaryOperation(Node const *rhs)
	: rhs(rhs)
{
	assert(nullptr != rhs);
}

UnaryOperation::~UnaryOperation()
{
	delete rhs;
}

///
///
///

BinaryOperation::BinaryOperation(Node const *lhs, Node const *rhs)
	: lhs(lhs) , rhs(rhs)
{
	assert(nullptr != lhs);
	assert(nullptr != rhs);
}

BinaryOperation::~BinaryOperation()
{
	delete lhs;
	delete rhs;
}

///
///
///

Negate::Negate(Node const *rhs)
	: UnaryOperation(rhs)
{
}

void Negate::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Add::Add(Node const *lhs, Node const *rhs)
	: BinaryOperation(lhs, rhs)
{
}

void Add::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Subtract::Subtract(Node const *lhs, Node const *rhs)
	: BinaryOperation(lhs, rhs)
{
}

void Subtract::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Multiply::Multiply(Node const *lhs, Node const *rhs)
	: BinaryOperation(lhs, rhs)
{
}

void Multiply::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Divide::Divide(Node const *lhs, Node const *rhs)
	: BinaryOperation(lhs, rhs)
{
}

void Divide::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Sine::Sine(Node const *rhs)
	: UnaryOperation(rhs)
{
}

void Sine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Cosine::Cosine(Node const *rhs)
	: UnaryOperation(rhs)
{
}

void Cosine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Exponential::Exponential(Node const *rhs)
	: UnaryOperation(rhs)
{
}

void Exponential::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Logarithm::Logarithm(Node const *rhs)
	: UnaryOperation(rhs)
{
}

void Logarithm::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Power::Power(Node const *lhs, Node const *rhs)
	: BinaryOperation(lhs, rhs)
{
}

void Power::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

LUCID_XPR_END