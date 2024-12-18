#pragma once

#include <vector>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Clone.h>
#include <lucid/xpr/Hasher.h>
#include <lucid/xpr/Rule.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;

///	Simplify
///
///	algebraic simplification.
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
class Simplify : public Algorithm
{
public:
	Simplify();

	virtual ~Simplify() = default;

	Node const *operator()(Node const *node);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Exponential const *node) override;

	virtual void evaluate(Logarithm const *node) override;

public:
	Clone clone;
	Hasher hash;

	std::vector<Rule<UnaryOperation> > unaryRules;
	std::vector<Rule<BinaryOperation> > binaryRules;

	Node const *simplified = nullptr;

	bool matches(Pattern const &pattern, Node const *ndoe);

	void addRule(Rule<UnaryOperation> const &rule);

	void addRule(Rule<BinaryOperation> const &rule);

	template<typename T> Node const *apply(std::vector<Rule<T> > const &rules, T const *node);

	template<typename T> Node const *lhs(T const *node);

	template<typename T> Node const *rhs(T const *node);

};

inline void Simplify::addRule(Rule<UnaryOperation> const &rule)
{
	unaryRules.push_back(rule);
}

inline void Simplify::addRule(Rule<BinaryOperation> const &rule)
{
	binaryRules.push_back(rule);
}

template<typename T> inline Node const *Simplify::apply(std::vector<Rule<T> > const &rules, T const *node)
{
	simplified = node;

	for (Rule<T> const &rule : rules)
	{
		if (matches(rule.pattern, simplified))
			simplified = rule.action(node);
	}

	return simplified;
}

template<typename T> inline Node const *Simplify::lhs(T const *node)
{
	node->lhs->apply(this);
	return simplified;
}

template<typename T> inline Node const *Simplify::rhs(T const *node)
{
	node->rhs->apply(this);
	return simplified;
}

LUCID_XPR_END