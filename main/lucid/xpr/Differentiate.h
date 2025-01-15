#pragma once

#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Clone.h>

LUCID_XPR_BEGIN

class Node;

///	Differentiate
///
///	computes the derivative of the supplied expression with
///	respect to the supplied symbol.
///
///	NOTE: does not alter the supplied expression.
class Differentiate : public Algorithm
{
public:
	Differentiate() = default;

	virtual ~Differentiate() = default;

	Node const *operator()(Node const *node, uint64_t index);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Function const *node) override;

	virtual void evaluate(Derivative const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Exponential const *node) override;

	virtual void evaluate(Logarithm const *node) override;

private:
	uint64_t index = -1;
	Node const *result = nullptr;

	template<typename T> Node const *u(T const *node);

	template<typename T> Node const *du(T const *node);

	template<typename T> Node const *v(T const *node);

	template<typename T> Node const *dv(T const *node);

};

template<typename T> inline Node const *Differentiate::u(T const *node)
{
	return clone(node->lhs);
}

template<typename T> inline Node const *Differentiate::du(T const *node)
{
	node->lhs->apply(this);
	return result;
}

template<typename T> inline Node const *Differentiate::v(T const *node)
{
	return clone(node->rhs);
}

template<typename T> inline Node const *Differentiate::dv(T const *node)
{
	node->rhs->apply(this);
	return result;
}

LUCID_XPR_END