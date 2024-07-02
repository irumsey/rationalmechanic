#pragma once

#include <string>
#include <sstream>
#include<lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Clone.h>
#include <lucid/xpr/Repr.h>

LUCID_XPR_BEGIN

class Node;

///	Simplify
///
///	algebraic simplification.  while this can be used
///	to create a "pretty" simplification, its main purpose
///	is to reduce "trivial" operations.
///
/// for example:
///		x + 0 -> x
///		x * 0 -> 0
///		x * 1 -> x
///		x / x -> 1
///		ln(e) -> 1
///		a + b -> c  (constant + constant -> constant)
///		etc...
///
///	NOTE: applies rules containing regular expressions
///	which are to match the output of Repr.
///
///	SEE ALSO: Repr
class Simplify : public Algorithm
{
public:
	Simplify() = default;

	virtual ~Simplify() = default;

	Node const *operator()(Node const *node);

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

public:
	Node const *result = nullptr;
	Clone clone;

	std::string repr(Node const *node) const;

	template<class T> Node const *lhsSimplify(T const *node);

	template<class T> Node const *rhsSimplify(T const *node);

};

inline std::string Simplify::repr(Node const *node) const
{
	std::ostringstream stream;
	Repr repr(stream);

	repr(node);

	return stream.str();
}

template<class T> inline Node const *Simplify::lhsSimplify(T const *node)
{
	node->lhs->apply(this);
	return result;
}

template<class T> inline Node const *Simplify::rhsSimplify(T const *node)
{
	node->rhs->apply(this);
	return result;
}

LUCID_XPR_END