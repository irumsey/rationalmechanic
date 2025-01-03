#pragma once

#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;

///	Clone
///
///	creates a "deep" copy of the supplied expression.
class Clone : public Algorithm
{
public:
	Clone() = default;

	virtual ~Clone() = default;

	Node const *operator()(Node const *node);

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

	virtual void evaluate(Power const *node) override;

private:
	Node const *result = nullptr;

	template<typename T> Node const *lhs(T const *node);

	template<typename T> Node const *rhs(T const *node);

};

template<class T> inline Node const *Clone::lhs(T const *node)
{
	node->lhs->apply(this);
	return result;
}

template<class T> inline Node const *Clone::rhs(T const *node)
{
	node->rhs->apply(this);
	return result;
}

LUCID_XPR_END