#include "Simplify.h"
#include "Node.h"
#include <regex>
#include <cassert>

LUCID_XPR_BEGIN

///	action
///
///	collection of actions used to simplify an expression.
///	typical actions involve deleting existing node(s) (lhs and/or rhs)
///	and replacing it/them with a new, simplified, node.
///	if an action returns null, the existing node(s) remain as is (ie no action occurs).
namespace action
{

	inline float64_t _value(Node const *node)
	{
		return static_cast<Constant const *>(node)->value;
	}

	inline std::string const &_symbol(Node const *node)
	{
		return static_cast<Variable const *>(node)->symbol;
	}

	Node const *replace_zero(Node const *lhs, Node const *rhs)
	{
		delete lhs;
		delete rhs;

		return val(0.0);
	}

	Node const *replace_one(Node const *lhs, Node const *rhs)
	{
		delete lhs;
		delete rhs;

		return val(1.0);
	}

	Node const *select_lhs(Node const *lhs, Node const *rhs)
	{
		delete rhs;

		return lhs;
	}

	Node const *select_rhs(Node const *lhs, Node const *rhs)
	{
		delete lhs;

		return rhs;
	}

	Node const *negate_rhs(Node const *lhs, Node const *rhs)
	{
		delete lhs;

		return neg(rhs);
	}

	Node const *compute_neg(Node const *rhs)
	{
		float64_t value = _value(rhs);

		delete rhs;

		return val(-value);
	}

	Node const *compute_ln(Node const *rhs)
	{
		float64_t value = ::log(_value(rhs));

		delete rhs;

		return val(value);
	}

	Node const *compute_sin(Node const *rhs)
	{
		float64_t value = ::sin(_value(rhs));

		delete rhs;

		return val(value);
	}

	Node const *compute_cos(Node const *rhs)
	{
		float64_t value = ::cos(_value(rhs));

		delete rhs;

		return val(value);
	}

	Node const *compute_add(Node const *lhs, Node const *rhs)
	{
		float64_t value = _value(lhs) + _value(rhs);

		delete lhs;
		delete rhs;

		return val(value);
	}

	Node const *compute_sub(Node const *lhs, Node const *rhs)
	{
		float64_t value = _value(lhs) - _value(rhs);

		delete lhs;
		delete rhs;

		return val(value);
	}

	Node const *compute_mul(Node const *lhs, Node const *rhs)
	{
		float64_t value = _value(lhs) * _value(rhs);

		delete lhs;
		delete rhs;

		return val(value);
	}

	Node const *compute_div(Node const *lhs, Node const *rhs)
	{
		float64_t value = _value(lhs) / _value(rhs);

		delete lhs;
		delete rhs;

		return val(value);
	}

	Node const *cancel(Node const *lhs, Node const *rhs)
	{
		if (_symbol(lhs) != _symbol(rhs))
		{
			return nullptr;
		}

		delete lhs;
		delete rhs;

		return val(1.0);
	}

	Node const *compute_pow(Node const *lhs, Node const *rhs)
	{
		float64_t value = ::pow(_value(lhs), _value(rhs));

		delete lhs;
		delete rhs;

		return val(value);
	}

}

///	UnaryRule
///
///	a regular expression and action to perform
///	if the "unary" expression matches the current node.
class UnaryRule
{
public:
	typedef Node const *(*action_t)(Node const *rhs);

	UnaryRule(std::regex const &regex, action_t action)
		: regex(regex)
		, action(action)
	{
		assert(nullptr != action);
	}

	virtual ~UnaryRule() = default;

	Node const *apply(std::string const &statement, Node const *rhs) const
	{
		if (std::regex_match(statement, regex))
		{
			return action(rhs);
		}
		return nullptr;
	}

private:
	std::regex regex;
	action_t action = nullptr;

};

///	BinaryRule
///
///	a regular expression and action to perform
///	if the "binary" expression matches the current node.
class BinaryRule
{
public:
	typedef Node const *(*action_t)(Node const *lhs, Node const *rhs);

	BinaryRule(std::regex const &regex, action_t action)
		: regex(regex)
		, action(action)
	{
		assert(nullptr != action);
	}

	virtual ~BinaryRule() = default;

	Node const *apply(std::string const &statement, Node const *lhs, Node const *rhs) const
	{
		if (std::regex_match(statement, regex))
		{
			return action(lhs, rhs);
		}
		return nullptr;
	}

private:
	std::regex regex;
	action_t action = nullptr;

};

///	Rules
///
///	collection of unary and binary rules to apply to an expression.
class Rules
{
public:
	typedef Node const *(*unary_oper_t)(Node const *rhs);
	typedef Node const *(*binary_oper_t)(Node const *lhs, Node const *rhs);

	virtual ~Rules() = default;

	static Node const *apply(std::string const &repr, unary_oper_t oper, Node const *rhs)
	{
		for (auto rule = instance.unaryRules.begin(); rule != instance.unaryRules.end(); ++rule)
		{
			if (Node const *simplified = rule->apply(repr, rhs))
			{
				return simplified;
			}
		}
		return oper(rhs);
	}

	static Node const *apply(std::string const &repr, binary_oper_t oper, Node const *lhs, Node const *rhs)
	{
		for (auto rule = instance.binaryRules.begin(); rule != instance.binaryRules.end(); ++rule)
		{
			if (Node const *simplified = rule->apply(repr, lhs, rhs))
			{
				return simplified;
			}
		}
		return oper(lhs, rhs);
	}

protected:
	Rules()
	{
		addRule("neg\\(val\\(.\\)\\)", action::compute_neg);

		addRule("ln\\(val\\(.\\)\\)", action::compute_ln);

		addRule("sin\\(val\\(.\\)\\)", action::compute_sin);

		addRule("cos\\(val\\(.\\)\\)", action::compute_cos);

		addRule("add\\((.*),val\\(0\\)\\)", action::select_lhs);
		addRule("add\\(val\\(0\\),(.*)\\)", action::select_rhs);
		addRule("add\\(val\\(.\\),val\\(.\\)\\)", action::compute_add);

		addRule("sub\\((.*),val\\(0\\)\\)", action::select_lhs);
		addRule("sub\\(val\\(0\\),(.*)\\)", action::negate_rhs);
		addRule("sub\\(val\\(.\\),val\\(.\\)\\)", action::compute_sub);

		addRule("mul\\((.*),val\\(0\\)\\)", action::replace_zero);
		addRule("mul\\(val\\(0\\),(.*)\\)", action::replace_zero);
		addRule("mul\\((.*),val\\(1\\)\\)", action::select_lhs);
		addRule("mul\\(val\\(1\\),(.*)\\)", action::select_rhs);
		addRule("mul\\(val\\(.\\),val\\(.\\)\\)", action::compute_mul);

		addRule("div\\((.*),val\\(1\\)\\)", action::select_lhs);
		addRule("div\\(val\\(0\\),(.*)\\)", action::replace_zero);
		addRule("div\\(val\\(.\\),val\\(.\\)\\)", action::compute_div);
		addRule("div\\(var\\(.\\),var\\(.\\)\\)", action::cancel);

		addRule("pow\\(val\\(.\\),val\\(.\\)\\)", action::compute_pow);
		addRule("pow\\((.*),val\\(0\\)\\)", action::replace_one);
		addRule("pow\\((.*),val\\(1\\)\\)", action::select_lhs);

#if 0
		///	it is possible to match sub-expressions for more complicated rules.
		///	however, this goes beyond the scope of "simple simplification".
		addRule("mul\\(val\\(.\\), mul\\(val\\(.\\), (.*)\\)\\)", action::some_complex_action);
		addRule("mul\\(val\\(.\\), mul\\((.*), val\\(.\\)\\)\\)", action::some_complex_action);
		addRule("mul\\(val\\(.\\), div\\(val\\(.\\), (.*)\\)\\)", action::some_complex_action);
#endif
	}

private:
	static Rules instance;

	std::vector<UnaryRule> unaryRules;
	std::vector<BinaryRule> binaryRules;

	void addRule(std::string const &statement, UnaryRule::action_t action)
	{
		unaryRules.push_back(UnaryRule(std::regex(statement), action));
	}

	void addRule(std::string const &statement, BinaryRule::action_t action)
	{
		binaryRules.push_back(BinaryRule(std::regex(statement), action));
	}

};

///	the one internal "static instance" of rules
Rules Rules::instance;

///
/// In the following, notice that the algorithm "works ahead" of the
/// current node. This means the argument(s) are simplified first.
/// Next, a preview representation of the current node is created
/// using the simplified argument(s). That representation is matched 
/// against the rules defined above. This allows the system to 
/// pre-emptively eliminate or otherwise alter the node if required.
/// 

Node const *Simplify::operator()(Node const *node)
{
	node->apply(this);
	return result;
}

void Simplify::evaluate(Constant const *node)
{
	result = clone(node);
}

void Simplify::evaluate(Variable const *node)
{
	result = clone(node);
}

void Simplify::evaluate(Negate const *node)
{
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "neg(";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, neg, rhs);
}

void Simplify::evaluate(NaturalLogarithm const *node)
{
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "ln(";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, ln, rhs);
}

void Simplify::evaluate(Sine const *node)
{
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "sin(";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, sin, rhs);
}

void Simplify::evaluate(Cosine const *node)
{
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "cos(";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, cos, rhs);
}

void Simplify::evaluate(Add const *node)
{
	Node const *lhs = lhsSimplify(node);
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "add(";
	simplified += repr(lhs);
	simplified += ",";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, add, lhs, rhs);
}

void Simplify::evaluate(Subtract const *node)
{
	Node const *lhs = lhsSimplify(node);
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "sub(";
	simplified += repr(lhs);
	simplified += ",";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, sub, lhs, rhs);
}

void Simplify::evaluate(Multiply const *node)
{
	Node const *lhs = lhsSimplify(node);
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "mul(";
	simplified += repr(lhs);
	simplified += ",";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, mul, lhs, rhs);
}

void Simplify::evaluate(Divide const *node)
{
	Node const *lhs = lhsSimplify(node);
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "div(";
	simplified += repr(lhs);
	simplified += ",";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, div, lhs, rhs);
}

void Simplify::evaluate(Power const *node)
{
	Node const *lhs = lhsSimplify(node);
	Node const *rhs = rhsSimplify(node);

	std::string simplified = "pow(";
	simplified += repr(lhs);
	simplified += ",";
	simplified += repr(rhs);
	simplified += ")";

	result = Rules::apply(simplified, pow, lhs, rhs);
}

LUCID_XPR_END