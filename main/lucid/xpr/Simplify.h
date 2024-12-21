#pragma once

#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Hasher.h>
#include <lucid/xpr/Rule.h>

LUCID_XPR_BEGIN

class Node;

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

	Node const *operator()(Node const *node, size_t passes = 2);

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

	virtual void evaluate(Power const *node) override;

public:
	std::vector<Rule> rules;
	Node const *simplified = nullptr;

	Node const *copy(Node const *node);

	Node const *simplify(Node const *node);

	bool matches(std::vector<Token> const &pattern, Node const *node) const;

	void addRule(Rule const &rule);

	Node const *applyRules(Node const *node);

	LUCID_PREVENT_COPY(Simplify);
	LUCID_PREVENT_ASSIGNMENT(Simplify);
};

LUCID_XPR_END