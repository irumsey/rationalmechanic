#include "Node.h"
#include "Hasher.h"
#include "Iterator.h"
#include "Clone.h"
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

Function::Function(uint64_t index, Signature const &signature)
	: index(index)
	, signature(signature)
{
}

void Function::apply(Algorithm *algorithm) const
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

Derivative::Derivative(Node const *arg, uint64_t index)
	: UnaryOperation(arg)
	, index(index)
{
}

void Derivative::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Negate::Negate(Node const *arg)
	: UnaryOperation(arg)
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

Sine::Sine(Node const *arg)
	: UnaryOperation(arg)
{
}

void Sine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Cosine::Cosine(Node const *arg)
	: UnaryOperation(arg)
{
}

void Cosine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Exponential::Exponential(Node const *arg)
	: UnaryOperation(arg)
{
}

void Exponential::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

Logarithm::Logarithm(Node const *arg)
	: UnaryOperation(arg)
{
}

void Logarithm::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
/// 

UnaryOperation const *pow(Node const *lhs, Node const *rhs)
{
	return exp(mul(rhs, log(lhs)));
}

BinaryOperation const *tan(Node const *arg)
{
	/// Note: making one _clone of arg to avoid
	/// it from having two parents.
	return div(sin(arg), cos(_clone(arg)));
}

BinaryOperation const *csc(Node const *arg)
{
	return div(val(1.0), sin(arg));
}

BinaryOperation const *sec(Node const *arg)
{
	return div(val(1.0), cos(arg));
}

BinaryOperation const *cot(Node const *arg)
{
	/// Note: making one _clone of arg to avoid
	/// it from having two parents.
	return div(cos(arg), sin(_clone(arg)));
}

bool _equal(Node const *lhs, Node const *rhs)
{
	Iterator lhsIter = lhs;
	Iterator rhsIter = rhs;

	while ((nullptr != lhsIter) && (nullptr != rhsIter))
	{
		if (_hash(lhsIter) != _hash(rhsIter))
			return false;
		++lhsIter;
		++rhsIter;
	}

	return ((nullptr == lhsIter) && (nullptr == rhsIter));
}

Node const *_swap(binary_func_t func, Node const *node)
{
	Node const *x = _clone(_lhs(node));
	Node const *y = _clone(_rhs(node));
	delete node;

	return func(y, x);
}

Node const *_sort(binary_func_t func, Node const *node)
{
	if (_index(_rhs(node)) < _index(_lhs(node)))
		return _swap(func, node);
	return node;
}

LUCID_XPR_END