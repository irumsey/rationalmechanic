#include "Node.h"
#include "Algorithm.h"
#include <cassert>

LUCID_XPR_BEGIN

//
///
/// 

Node::Node(size_t tid)
	: tid(tid)
{
}

///
///
///

size_t Any::TID()
{
	return size_t(&TID);
}

Any::Any()
	: Node(TID())
{
}

void Any::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Constant::TID()
{
	return size_t(&TID);
}

Constant::Constant(float64_t const &value)
	: Node(TID())
	, value(value)
{
}

void Constant::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Variable::TID()
{
	return size_t(&TID);
}

Variable::Variable(std::string const &symbol)
	: Node(TID())
	, symbol(symbol)
{
}

void Variable::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

UnaryOperation::UnaryOperation(size_t tid, Node const *rhs)
	: Node(tid)
	, rhs(rhs)
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

BinaryOperation::BinaryOperation(size_t tid, Node const *lhs, Node const *rhs)
	: Node(tid)
	, lhs(lhs) , rhs(rhs)
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

size_t Negate::TID()
{
	return size_t(&TID);
}

Negate::Negate(Node const *rhs)
	: UnaryOperation(TID(), rhs)
{
}

void Negate::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t NaturalLogarithm::TID()
{
	return size_t(&TID);
}

NaturalLogarithm::NaturalLogarithm(Node const *rhs)
	: UnaryOperation(TID(), rhs)
{
}

void NaturalLogarithm::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Sine::TID()
{
	return size_t(&TID);
}

Sine::Sine(Node const *rhs)
	: UnaryOperation(TID(), rhs)
{
}

void Sine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Cosine::TID()
{
	return size_t(&TID);
}

Cosine::Cosine(Node const *rhs)
	: UnaryOperation(TID(), rhs)
{
}

void Cosine::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Add::TID()
{
	return size_t(&TID);
}

Add::Add(Node const *lhs, Node const *rhs)
	: BinaryOperation(TID(), lhs, rhs)
{
}

void Add::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Subtract::TID()
{
	return size_t(&TID);
}

Subtract::Subtract(Node const *lhs, Node const *rhs)
	: BinaryOperation(TID(), lhs, rhs)
{
}

void Subtract::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Multiply::TID()
{
	return size_t(&TID);
}

Multiply::Multiply(Node const *lhs, Node const *rhs)
	: BinaryOperation(TID(), lhs, rhs)
{
}

void Multiply::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}


///
///
///

size_t Divide::TID()
{
	return size_t(&TID);
}

Divide::Divide(Node const *lhs, Node const *rhs)
	: BinaryOperation(TID(), lhs, rhs)
{
}

void Divide::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

///
///
///

size_t Power::TID()
{
	return size_t(&TID);
}

Power::Power(Node const *lhs, Node const *rhs)
	: BinaryOperation(TID(), lhs, rhs)
{
}

void Power::apply(Algorithm *algorithm) const
{
	algorithm->evaluate(this);
}

LUCID_XPR_END