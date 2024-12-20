#include "Simplify.h"
#include "Hasher.h"
#include "Clone.h"
#include "Iterator.h"
#include "Node.h"
#include <cassert>

LUCID_XPR_BEGIN

Token const ANY	= Token(             size_t(-1));
Token const NEG = Token(TYPE<     Negate>::ID());
Token const ADD = Token(TYPE<        Add>::ID());
Token const SUB	= Token(TYPE<   Subtract>::ID());
Token const MUL = Token(TYPE<   Multiply>::ID());
Token const DIV = Token(TYPE<     Divide>::ID());
Token const SIN = Token(TYPE<       Sine>::ID());
Token const COS = Token(TYPE<     Cosine>::ID());
Token const EXP = Token(TYPE<Exponential>::ID());
Token const LOG = Token(TYPE<  Logarithm>::ID());
Token const POW = Token(TYPE<      Power>::ID());

Token VAL()                { return Token(TYPE<Constant>::ID()       ); }
Token VAL(float64_t value) { return Token(TYPE<Constant>::ID(), value); }
Token VAR( uint64_t index) { return Token(TYPE<Variable>::ID(), index); }

///
///
///

bool are_identical(Node const *lhs, Node const *rhs)
{
	thread_local static Hasher hash;

	Iterator lhsIter = lhs;
	Iterator rhsIter = rhs;

	while ((nullptr != lhsIter) && (nullptr != rhsIter))
	{
		if (hash(lhsIter.ptr()) != hash(rhsIter.ptr()))
			return false;
		++lhsIter;
		++rhsIter;
	}

	return ((nullptr == lhsIter) && (nullptr == rhsIter));
}

bool are_not_identical(Node const *lhs, Node const *rhs)
{
	return !are_identical(lhs, rhs);
}

Node const *copy(Node const *node)
{
	thread_local static Clone clone;
	return clone(node);
}

float64_t value(Node const *node)
{
	return reinterpret_cast<Constant const *>(node)->value;
}

uint64_t index(Node const *node)
{
	return reinterpret_cast<Variable const *>(node)->index;
}

template<typename T> T const *rhs_as(UnaryOperation const *node)
{
	return reinterpret_cast<T const *>(node->rhs);
}

template<typename T> T const *lhs_as(BinaryOperation const *node)
{
	return reinterpret_cast<T const *>(node->lhs);
}

template<typename T> T const *rhs_as(BinaryOperation const *node)
{
	return reinterpret_cast<T const *>(node->rhs);
}

Node const *set_to_zero(UnaryOperation const *node)
{
	delete node;
	return val(0);
}

Node const *set_to_zero(BinaryOperation const *node)
{
	delete node;
	return val(0);
}

Node const *set_to_one(BinaryOperation const *node)
{
	delete node;
	return val(1);
}

Node const *select_lhs(BinaryOperation const *node)
{
	Node const *copied = copy(node->lhs);
	delete node;

	return copied;
}

Node const *select_rhs(BinaryOperation const *node)
{
	Node const *copied = copy(node->rhs);
	delete node;

	return copied;
}

Node const *negate_rhs(BinaryOperation const *node)
{
	Node const *copied = copy(node->rhs);
	delete node;

	return neg(copied);
}

Node const *compute_neg(UnaryOperation const *node)
{
	float64_t result = -value(node->rhs);
	delete node;

	return val(result);
}

Node const *compute_add(BinaryOperation const *node)
{
	float64_t result = value(node->lhs) + value(node->rhs);
	delete node;

	return val(result);
}

Node const *compute_sub(BinaryOperation const *node)
{
	float64_t result = value(node->lhs) - value(node->rhs);
	delete node;

	return val(result);
}

Node const *compute_mul(BinaryOperation const *node)
{
	float64_t result = value(node->lhs) * value(node->rhs);
	delete node;

	return val(result);
}

Node const *compute_div(BinaryOperation const *node)
{
	float64_t result = value(node->lhs) / value(node->rhs);
	delete node;

	return val(result);
}

Node const *compute_sin(UnaryOperation const *node)
{
	float64_t result = ::sin(value(node->rhs));
	delete node;

	return val(result);
}

Node const *compute_cos(UnaryOperation const *node)
{
	float64_t result = ::cos(value(node->rhs));
	delete node;

	return val(result);
}

Node const *compute_exp(UnaryOperation const *node)
{
	float64_t result = ::exp(value(node->rhs));
	delete node;

	return val(result);
}

Node const *compute_log(UnaryOperation const *node)
{
	float64_t result = ::log(value(node->rhs));
	delete node;

	return val(result);
}

Node const *collapse_identity(UnaryOperation const *node)
{
	Node const *copied = copy(rhs_as<Node>(rhs_as<UnaryOperation>(node)));
	delete node;

	return copied;
}

Node const *cancel_terms(BinaryOperation const *node)
{
	if (are_not_identical(node->lhs, node->rhs))
		return node;
	delete node;

	return val(1);
}

Node const *cancel_power_terms(BinaryOperation const *node)
{
	Node const *x = lhs_as<Node>(lhs_as<BinaryOperation>(node));
	Node const *y = rhs_as<Node>(node);

	if (are_not_identical(x, y))
		return node;

	Node const *n = rhs_as<Node>(lhs_as<BinaryOperation>(node));
	Node const *canceled = pow(copy(x), sub(copy(n), val(1)));
	delete node;

	return canceled;
}

///
///
///

Simplify::Simplify()
{
	typedef Rule< UnaryOperation>  unary_rule_t;
	typedef Rule<BinaryOperation> binary_rule_t;

	addRule( unaryRules,  unary_rule_t( { NEG, VAL(0),         },  set_to_zero));
	addRule( unaryRules,  unary_rule_t( { NEG,    NEG,    ANY, }, collapse_identity));

	addRule(binaryRules, binary_rule_t( { ADD,    ANY, VAL(0), },   select_lhs));
	addRule(binaryRules, binary_rule_t( { ADD, VAL(0),    ANY, },   select_rhs));

	addRule(binaryRules, binary_rule_t( { SUB,    ANY, VAL(0), },   select_lhs));
	addRule(binaryRules, binary_rule_t( { SUB, VAL(0),    ANY, },   negate_rhs));

	addRule(binaryRules, binary_rule_t( { MUL,    ANY, VAL(0), },  set_to_zero));
	addRule(binaryRules, binary_rule_t( { MUL, VAL(0),    ANY, },  set_to_zero));
	addRule(binaryRules, binary_rule_t( { MUL,    ANY, VAL(1), },   select_lhs));
	addRule(binaryRules, binary_rule_t( { MUL, VAL(1),    ANY, },   select_rhs));

	addRule(binaryRules, binary_rule_t( { DIV,    ANY,    ANY, }, cancel_terms));
	addRule(binaryRules, binary_rule_t( { DIV, VAL(0),    ANY, },  set_to_zero));
	addRule(binaryRules, binary_rule_t( { DIV,    ANY, VAL(1), },   select_lhs));

	addRule( unaryRules,  unary_rule_t( { EXP,    LOG,    ANY, }, collapse_identity));
	addRule( unaryRules,  unary_rule_t( { LOG,    EXP,    ANY, }, collapse_identity));

	addRule(binaryRules, binary_rule_t( { POW,    ANY, VAL(0), },  set_to_one));
	addRule(binaryRules, binary_rule_t( { POW,    ANY, VAL(1), },  select_lhs));

	addRule(binaryRules, binary_rule_t( { DIV,    POW,    ANY,    ANY,    ANY, }, cancel_power_terms)); 

	addRule( unaryRules,  unary_rule_t( { NEG,  VAL(),         },  compute_neg));
	addRule( unaryRules,  unary_rule_t( { SIN,  VAL(),         },  compute_sin));
	addRule( unaryRules,  unary_rule_t( { COS,  VAL(),         },  compute_cos));
	addRule( unaryRules,  unary_rule_t( { EXP,  VAL(),         },  compute_exp));
	addRule( unaryRules,  unary_rule_t( { LOG,  VAL(),         },  compute_log));
	addRule(binaryRules, binary_rule_t( { ADD,  VAL(),  VAL(), },  compute_add));
	addRule(binaryRules, binary_rule_t( { SUB,  VAL(),  VAL(), },  compute_sub));
	addRule(binaryRules, binary_rule_t( { MUL,  VAL(),  VAL(), },  compute_mul));
	addRule(binaryRules, binary_rule_t( { DIV,  VAL(),  VAL(), },  compute_div));
}			

Node const *Simplify::operator()(Node const *node)
{
	simplified = node;
	simplified->apply(this);
	simplified->apply(this);
	return simplified;
}

void Simplify::evaluate(Constant const *node)
{
	thread_local static Clone clone;
	simplified = clone(node);
}

void Simplify::evaluate(Variable const *node)
{
	thread_local static Clone clone;
	simplified = clone(node);
}

void Simplify::evaluate(Negate const *node)
{
	simplified = apply(unaryRules, neg(rhs(node)));
}

void Simplify::evaluate(Add const *node)
{
	simplified = apply(binaryRules, add(lhs(node), rhs(node)));
}

void Simplify::evaluate(Subtract const *node)
{
	simplified = apply(binaryRules, sub(lhs(node), rhs(node)));
}

void Simplify::evaluate(Multiply const *node)
{
	simplified = apply(binaryRules, mul(lhs(node), rhs(node)));
}

void Simplify::evaluate(Divide const *node)
{
	simplified = apply(binaryRules, div(lhs(node), rhs(node)));
}

void Simplify::evaluate(Sine const *node)
{
	simplified = apply(unaryRules, sin(rhs(node)));
}

void Simplify::evaluate(Cosine const *node)
{
	simplified = apply(unaryRules, cos(rhs(node)));
}

void Simplify::evaluate(Exponential const *node)
{
	simplified = apply(unaryRules, exp(rhs(node)));
}

void Simplify::evaluate(Logarithm const *node)
{
	simplified = apply(unaryRules, log(rhs(node)));
}

void Simplify::evaluate(Power const *node)
{
	simplified = apply(binaryRules, pow(lhs(node), rhs(node)));
}

bool Simplify::matches(Pattern const &pattern, Node const *node)
{
	thread_local static Hasher hash;

	auto lhs = pattern.begin();
	Iterator rhs = node;

	while ((pattern.end() != lhs) && (nullptr != rhs))
	{
		if (ANY == *lhs)
		{
			++lhs;
			rhs.skip();
			continue;
		}

		if (*lhs != hash(rhs.ptr()))
		{
			return false;
		}

		++lhs;
		++rhs;
	}

	return ((pattern.end() == lhs) && (nullptr == rhs));
}

LUCID_XPR_END