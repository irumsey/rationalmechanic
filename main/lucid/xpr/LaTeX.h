#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Rule.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;
class Registry;

///	LaTeX
///
/// 
class LaTeX : public Algorithm
{
public:
	LaTeX();

	LaTeX(Node const *node, Registry const &registry);

	virtual ~LaTeX() = default;

	std::string operator()(Node const *node, Registry const &registry);

	std::string format(Node const *node, Registry const &registry);

	std::string str() const;

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
	typedef void (LaTeX::*action_t)(Node const *);
	typedef Rule<action_t> rule_t;

	Registry const *symbols = nullptr;
	std::vector<rule_t> rules;

	std::ostringstream stream;

	void initialize();

	void addRule(rule_t const &rule);

	void applyRules(Node const *node);

	/// 
	/// actions
	/// 

	void format_neg(Node const *node);

	void format_neg_default(Node const *node);

	void format_mul_0(Node const *node);

	void format_mul_1(Node const *node);

	void format_mul_2(Node const *node);

	void format_mul_default(Node const *node);

	void format_ddx(Node const *node);

	void format_ddx_default(Node const *node);

};

inline std::string LaTeX::str() const
{
	return stream.str();
}

inline void LaTeX::addRule(rule_t const &rule)
{
	rules.push_back(rule);
}

inline void LaTeX::applyRules(Node const *node)
{
	for (rule_t const &rule : rules)
	{
		if (rule.matches(node))
		{
			action_t action = rule.action;
			(this->*action)(node);
			return;
		}
	}

	/// TBD: should not get here...
	assert(false);
}

inline std::string _LaTeX(Node const *node, Registry const &registry)
{
	thread_local static LaTeX format;
	return format(node, registry);
}

LUCID_XPR_END

inline std::ostream &operator<<(std::ostream &stream, LUCID_XPR::LaTeX const &repr)
{
	stream << repr.str();
	return stream;
}
