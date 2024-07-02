#pragma once

#include <string>
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

	Node const *operator()(Node const *node, std::string const &symbol);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(NaturalLogarithm const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Power const *node) override;

private:
	std::string symbol;
	Node const *result = nullptr;

	template<class T> Node const *u(T const *node) const;

	template<class T> Node const *du(T const *node);

	template<class T> Node const *v(T const *node) const;

	template<class T> Node const *dv(T const *node);

};

template<class T> inline Node const *Differentiate::u(T const *node) const
{
	Clone clone;
	return clone(node->lhs);
}

template<class T> inline Node const *Differentiate::du(T const *node)
{
	node->lhs->apply(this);
	return result;
}

template<class T> inline Node const *Differentiate::v(T const *node) const
{
	Clone clone;
	return clone(node->rhs);
}

template<class T> inline Node const *Differentiate::dv(T const *node)
{
	node->rhs->apply(this);
	return result;
}

LUCID_XPR_END