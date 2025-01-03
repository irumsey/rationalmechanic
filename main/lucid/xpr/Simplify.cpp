#include "Simplify.h"
#include "Clone.h"
#include "Iterator.h"
#include "Action.h"
#include "Node.h"
#include "Registry.h"
#include <cassert>

LUCID_XPR_BEGIN

///
///
///

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

Token VAR()                { return Token(TYPE<Variable>::ID()       ); }
Token VAR( uint64_t index) { return Token(TYPE<Variable>::ID(), index); }

///
///
///

Simplify::Simplify()
{
	///
	/// for the following, order matters.  for example: moving constants
	/// to the right, when possible, means following rules only need to
	/// test for right hand constants.
	/// 

	///
	///	traversal of the tree is left node first, therefore, move constants
	/// to the right. this accomplishes two things:
	///		first, differs computation allowing for constants to accumulate.
	///		second, reduces the total number of rules.
	/// 
	addRule(Rule( { ADD,  VAL(),    ANY, }, action::    swap_add_args));
	addRule(Rule( { MUL,  VAL(),    ANY, }, action::    swap_mul_args));

	///
	///	sort variables in an attempt to accumulate like terms.
	/// 
	addRule(Rule( { ADD,  VAR(),  VAR(), }, action::    sort_add_args));
	addRule(Rule( { MUL,  VAR(),  VAR(), }, action::    sort_mul_args));

	addRule(Rule( { NEG, VAL(0),         }, action::             zero));
	addRule(Rule( { NEG,    NEG,    ANY, }, action::collapse_identity));

	addRule(Rule( { ADD,    ANY, VAL(0), }, action::       select_lhs));
	addRule(Rule( { ADD,    ANY,    ANY, }, action::     factor_terms));

	addRule(Rule( { SUB,    ANY, VAL(0), }, action::       select_lhs));
	addRule(Rule( { SUB, VAL(0),    ANY, }, action::       negate_rhs));
	addRule(Rule( { SUB,    ANY,    ANY, }, action::       sub_cancel));

	addRule(Rule( { MUL,    ANY, VAL(0), }, action::             zero));
	addRule(Rule( { MUL,    ANY, VAL(1), }, action::       select_lhs));

	addRule(Rule( { DIV,    ANY,    ANY, }, action::       div_cancel));
	addRule(Rule( { DIV, VAL(0),    ANY, }, action::             zero));
	addRule(Rule( { DIV,    ANY, VAL(1), }, action::       select_lhs));

	addRule(Rule( { DIV,    ANY,    DIV,    ANY,    ANY, }, action::inv_and_mul));

	addRule(Rule( { EXP,    LOG,    ANY, }, action::collapse_identity));
	addRule(Rule( { LOG,    EXP,    ANY, }, action::collapse_identity));

	addRule(Rule( { POW,    ANY, VAL(0), }, action::              one));
	addRule(Rule( { POW,    ANY, VAL(1), }, action::       select_lhs));

	addRule(Rule( { DIV,    POW,    ANY,    ANY,    ANY, }, action::div_pow_cancel)); 

	///
	///	lastly, compute everthing containing constants (if possible).
	/// 
	addRule(Rule( { MUL,    DIV,  VAL(),    ANY,  VAL(), }, action::mul_combine_1));
	addRule(Rule( { DIV,    MUL,    ANY,  VAL(),  VAL(), }, action::div_combine_1));
	addRule(Rule( { MUL,    DIV,    ANY,  VAL(),  VAL(), }, action::div_combine_2));

	addRule(Rule( { NEG,  VAL(),         }, action::      compute_neg));
	addRule(Rule( { SIN,  VAL(),         }, action::      compute_sin));
	addRule(Rule( { COS,  VAL(),         }, action::      compute_cos));
	addRule(Rule( { EXP,  VAL(),         }, action::      compute_exp));
	addRule(Rule( { LOG,  VAL(),         }, action::      compute_log));
	addRule(Rule( { ADD,  VAL(),  VAL(), }, action::      compute_add));
	addRule(Rule( { SUB,  VAL(),  VAL(), }, action::      compute_sub));
	addRule(Rule( { MUL,  VAL(),  VAL(), }, action::      compute_mul));
	addRule(Rule( { DIV,  VAL(),  VAL(), }, action::      compute_div));
	addRule(Rule( { EXP,  VAL(),         }, action::      compute_exp));
	addRule(Rule( { LOG,  VAL(),         }, action::      compute_log));
	addRule(Rule( { POW,  VAL(),  VAL(), }, action::      compute_pow));

	/// the ANYs should not be a constant (since it would have been computed above).
	addRule(Rule( { ADD,    ADD,    ANY,  VAL(),  VAL(), }, action::collapse_addition));
	addRule(Rule( { MUL,    MUL,    ANY,  VAL(),  VAL(), }, action::collapse_multiplication));
}								

Node const *Simplify::operator()(Node const *node, size_t passes)
{
	return simplify(node, nullptr, passes);
}

Node const *Simplify::operator()(Node const *node, Registry const &registry, size_t passes)
{
	return simplify(node, &registry, passes);
}

void Simplify::evaluate(Constant const *node)
{
	simplified = copy(node);
}

void Simplify::evaluate(Variable const *node)
{
	simplified = (nullptr != symbols) ? val(symbols->value_of(node->index)) : copy(node);
}

void Simplify::evaluate(Function const *node)
{
	simplified = copy(node);
}

void Simplify::evaluate(Derivative const *node)
{
	simplified = copy(node);
}

void Simplify::evaluate(Negate const *node)
{
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(neg(rhs));
}

void Simplify::evaluate(Add const *node)
{
	Node const *lhs = simplify(node->lhs);
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(add(lhs, rhs));
}

void Simplify::evaluate(Subtract const *node)
{
	Node const *lhs = simplify(node->lhs);
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(sub(lhs, rhs));
}

void Simplify::evaluate(Multiply const *node)
{
	Node const *lhs = simplify(node->lhs);
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(mul(lhs, rhs));
}

void Simplify::evaluate(Divide const *node)
{
	Node const *lhs = simplify(node->lhs);
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(div(lhs, rhs));
}

void Simplify::evaluate(Sine const *node)
{
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(sin(rhs));
}

void Simplify::evaluate(Cosine const *node)
{
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(cos(rhs));
}

void Simplify::evaluate(Exponential const *node)
{
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(exp(rhs));
}

void Simplify::evaluate(Logarithm const *node)
{
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(log(rhs));
}

void Simplify::evaluate(Power const *node)
{
	Node const *lhs = simplify(node->lhs);
	Node const *rhs = simplify(node->rhs);

	simplified = applyRules(pow(lhs, rhs));
}

Node const *Simplify::copy(Node const *node)
{
	thread_local static Clone clone;
	return clone(node);
}

Node const *Simplify::simplify(Node const *node, Registry const *registry, size_t passes)
{
	symbols = registry;

	Node const *result = simplify(node); 
	for (size_t pass = 1; pass < passes; ++pass)
	{
		Node const *tmp = simplify(result);
		delete result;
		result = tmp;
	}
	
	symbols = nullptr;

	return result;
}

Node const *Simplify::simplify(Node const *node)
{
	node->apply(this);
	return simplified;
}

bool Simplify::matches(std::vector<Token> const &pattern, Node const *node) const
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