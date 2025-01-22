#pragma once

#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Hasher.h>
#include <lucid/xpr/Rule.h>

LUCID_XPR_BEGIN

class Registry;
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
/// note: if a symbol registry is supplied, the simplification
///       operation also includes variable substitution.
/// note: if a symbol registry is supplied, all variables must
///       have a value assigned.  user defined functions remain
///       in symbolic form.
class Simplify : public Algorithm
{
public:
	Simplify();

	virtual ~Simplify() = default;

	Node const *operator()(Node const *node, size_t passes = 2);

	Node const *operator()(Node const *node, Registry const &registry, size_t passes = 2);

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

public:
	typedef Node const *(Simplify::*action_t)(Node const *);
	typedef Rule<action_t> rule_t;

	Registry const *symbols = nullptr;

	std::vector<rule_t> rules;
	Node const *simplified = nullptr;

	Node const *simplify(Node const *node, Registry const *registry, size_t passes);

	Node const *simplify(Node const *node);

	void addRule(rule_t const &rule);

	Node const *applyRules(Node const *node);

	/// 
	///	actions
	/// 

	Node const *nop(Node const *node);

	Node const *zero(Node const *node);

	Node const *one(Node const *node);

	Node const *select_arg(Node const *node);

	Node const *select_lhs(Node const *node);

	Node const *select_rhs(Node const *node);

	Node const *negate_rhs(Node const *node);

	Node const *swap_add_args(Node const *node);

	Node const *sort_add_args(Node const *node);

	Node const *swap_mul_args(Node const *node);

	Node const *sort_mul_args(Node const *node);

	Node const *inv_and_mul(Node const *node);

	Node const *factor_terms(Node const *node);

	Node const *sub_cancel(Node const *node);

	Node const *compute_neg(Node const *node);

	Node const *compute_add(Node const *node);

	Node const *collapse_addition(Node const *node);

	Node const *compute_sub(Node const *node);

	Node const *compute_mul(Node const *node);

	Node const *mul_combine_1(Node const *node);

	Node const *div_combine_1(Node const *node);

	Node const *div_combine_2(Node const *node);

	Node const *collapse_multiplication(Node const *node);

	Node const *compute_div(Node const *node);

	Node const *compute_sin(Node const *node);

	Node const *compute_cos(Node const *node);

	Node const *compute_exp(Node const *node);

	Node const *compute_log(Node const *node);

	Node const *compute_pow(Node const *node);

	Node const *collapse_identity(Node const *node);

	Node const *div_cancel(Node const *node);

	Node const *div_pow_cancel(Node const *node);

	Node const *ddx_function(Node const *node);

	LUCID_PREVENT_COPY(Simplify);
	LUCID_PREVENT_ASSIGNMENT(Simplify);
};

inline void Simplify::addRule(rule_t const &rule)
{
	rules.push_back(rule);
}

inline Node const *Simplify::applyRules(Node const *node)
{
	simplified = node;

	for (rule_t const &rule : rules)
	{
		if (rule.matches(simplified))
		{
			action_t action = rule.action;
			simplified = (this->*action)(simplified);
		}
	}

	return simplified;
}

inline Node const *_simplify(Node const *node, size_t passes = 2)
{
	thread_local static Simplify simplify;
	return simplify(node, passes);
}

LUCID_XPR_END