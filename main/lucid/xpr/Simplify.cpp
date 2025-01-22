#include "Simplify.h"
#include "Clone.h"
#include "Iterator.h"
#include "Node.h"
#include "Registry.h"
#include <cassert>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

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
	addRule(rule_t( { Token::ADD,  Token::VAL(),    Token::ANY, },     &Simplify::swap_add_args));
	addRule(rule_t( { Token::MUL,  Token::VAL(),    Token::ANY, },     &Simplify::swap_mul_args));

	///
	///	sort variables in an attempt to accumulate like terms.
	/// 
	addRule(rule_t( { Token::ADD,  Token::VAR(),  Token::VAR(), },     &Simplify::sort_add_args));
	addRule(rule_t( { Token::MUL,  Token::VAR(),  Token::VAR(), },     &Simplify::sort_mul_args));

	addRule(rule_t( { Token::NEG, Token::VAL(0),         },                     &Simplify::zero));
	addRule(rule_t( { Token::NEG,    Token::NEG,    Token::ANY, }, &Simplify::collapse_identity));

	addRule(rule_t( { Token::ADD,    Token::ANY, Token::VAL(0), },        &Simplify::select_lhs));
	addRule(rule_t( { Token::ADD,    Token::ANY,    Token::ANY, },      &Simplify::factor_terms));

	addRule(rule_t( { Token::SUB,    Token::ANY, Token::VAL(0), },        &Simplify::select_lhs));
	addRule(rule_t( { Token::SUB, Token::VAL(0),    Token::ANY, },        &Simplify::negate_rhs));
	addRule(rule_t( { Token::SUB,    Token::ANY,    Token::ANY, },        &Simplify::sub_cancel));

	addRule(rule_t( { Token::MUL,    Token::ANY, Token::VAL(0), },              &Simplify::zero));
	addRule(rule_t( { Token::MUL,    Token::ANY, Token::VAL(1), },        &Simplify::select_lhs));

	addRule(rule_t( { Token::DIV,    Token::ANY,    Token::ANY, },        &Simplify::div_cancel));
	addRule(rule_t( { Token::DIV, Token::VAL(0),    Token::ANY, },              &Simplify::zero));
	addRule(rule_t( { Token::DIV,    Token::ANY, Token::VAL(1), },        &Simplify::select_lhs));

	addRule(rule_t( { Token::DIV,    Token::ANY,    Token::DIV,    Token::ANY,    Token::ANY, }, &Simplify::inv_and_mul));

	addRule(rule_t( { Token::EXP,    Token::LOG,    Token::ANY, }, &Simplify::collapse_identity));
	addRule(rule_t( { Token::LOG,    Token::EXP,    Token::ANY, }, &Simplify::collapse_identity));

	addRule(rule_t( { Token::DDX,  Token::VAL(),                },              &Simplify::zero));
	addRule(rule_t( { Token::DDX,  Token::   FN,                },      &Simplify::ddx_function));

	///
	///	lastly, compute everthing containing constants (if possible).
	/// 
	addRule(rule_t( { Token::MUL,    Token::DIV,  Token::VAL(),    Token::ANY,  Token::VAL(), }, &Simplify::mul_combine_1));
	addRule(rule_t( { Token::DIV,    Token::MUL,    Token::ANY,  Token::VAL(),  Token::VAL(), }, &Simplify::div_combine_1));
	addRule(rule_t( { Token::MUL,    Token::DIV,    Token::ANY,  Token::VAL(),  Token::VAL(), }, &Simplify::div_combine_2));

	addRule(rule_t( { Token::NEG,  Token::VAL(),                },       &Simplify::compute_neg));
	addRule(rule_t( { Token::SIN,  Token::VAL(),                },       &Simplify::compute_sin));
	addRule(rule_t( { Token::COS,  Token::VAL(),                },       &Simplify::compute_cos));
	addRule(rule_t( { Token::EXP,  Token::VAL(),                },       &Simplify::compute_exp));
	addRule(rule_t( { Token::LOG,  Token::VAL(),                },       &Simplify::compute_log));
	addRule(rule_t( { Token::ADD,  Token::VAL(),  Token::VAL(), },       &Simplify::compute_add));
	addRule(rule_t( { Token::SUB,  Token::VAL(),  Token::VAL(), },       &Simplify::compute_sub));
	addRule(rule_t( { Token::MUL,  Token::VAL(),  Token::VAL(), },       &Simplify::compute_mul));
	addRule(rule_t( { Token::DIV,  Token::VAL(),  Token::VAL(), },       &Simplify::compute_div));
	addRule(rule_t( { Token::EXP,  Token::VAL(),                },       &Simplify::compute_exp));
	addRule(rule_t( { Token::LOG,  Token::VAL(),                },       &Simplify::compute_log));
													        
	/// the ANYs should not be a constant (since it would have been computed above).
	addRule(rule_t( { Token::ADD,    Token::ADD,    Token::ANY,  Token::VAL(),  Token::VAL(), }, &Simplify::collapse_addition));
	addRule(rule_t( { Token::MUL,    Token::MUL,    Token::ANY,  Token::VAL(),  Token::VAL(), }, &Simplify::collapse_multiplication));
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
	simplified = _clone(node);
}

void Simplify::evaluate(Variable const *node)
{
	simplified = (nullptr != symbols) ? val(symbols->value_of(node->index)) : _clone(node);
}

void Simplify::evaluate(Function const *node)
{
	simplified = _clone(node);
}

void Simplify::evaluate(Derivative const *node)
{
	simplified = _clone(node);
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

/// 
/// 
/// 

Node const *Simplify::nop(Node const *node)
{
	return node;
}

Node const *Simplify::zero(Node const *node)
{
	delete node;
	return val(0);
}

Node const *Simplify::one(Node const *node)
{
	delete node;
	return val(1);
}

Node const *Simplify::select_arg(Node const *node)
{
	Node const *copied = _clone(_arg(node));
	delete node;

	return copied;
}

Node const *Simplify::select_lhs(Node const *node)
{
	Node const *copied = _clone(_lhs(node));
	delete node;

	return copied;
}

Node const *Simplify::select_rhs(Node const *node)
{
	Node const *copied = _clone(_rhs(node));
	delete node;

	return copied;
}

Node const *Simplify::negate_rhs(Node const *node)
{
	Node const *copied = _clone(_rhs(node));
	delete node;

	return neg(copied);
}

Node const *Simplify::swap_add_args(Node const *node)
{
	return _swap(add, node);
}

Node const *Simplify::sort_add_args(Node const *node)
{
	return _sort(add, node);
}

Node const *Simplify::swap_mul_args(Node const *node)
{
	return _swap(mul, node);
}

Node const *Simplify::sort_mul_args(Node const *node)
{
	return _sort(mul, node);
}

Node const *Simplify::inv_and_mul(Node const *node)
{
	Node const *x = _clone(_lhs(node));
	Node const *y = _clone(_lhs(_rhs(node)));
	Node const *z = _clone(_rhs(_rhs(node)));
	delete node;

	return mul(x, div(z, y));
}

Node const *Simplify::factor_terms(Node const *node)
{
	if (!_equal(_lhs(node), _rhs(node)))
		return node;

	Node const *result = mul(val(2), _clone(_lhs(node)));
	delete node;

	return result;
}

Node const *Simplify::sub_cancel(Node const *node)
{
	if (!_equal(_lhs(node), _rhs(node)))
		return node;
	delete node;

	return val(0);
}

Node const *Simplify::compute_neg(Node const *node)
{
	float64_t result = -_value(_arg(node));
	delete node;

	return val(result);
}

Node const *Simplify::compute_add(Node const *node)
{
	float64_t result = _value(_lhs(node)) + _value(_rhs(node));
	delete node;

	return val(result);
}

Node const *Simplify::collapse_addition(Node const *node)
{
	Node const *x = _clone (_lhs(_lhs(node)));
	float64_t   a = _value(_rhs(_lhs(node)));
	float64_t   b = _value(_rhs(node));
	delete node;

	return add(x, val(a + b));
}

Node const *Simplify::compute_sub(Node const *node)
{
	float64_t result = _value(_lhs(node)) - _value(_rhs(node));
	delete node;

	return val(result);
}

Node const *Simplify::compute_mul(Node const *node)
{
	float64_t result = _value(_lhs(node)) * _value(_rhs(node));
	delete node;

	return val(result);
}

Node const *Simplify::mul_combine_1(Node const *node)
{
	float64_t k = _value(_lhs(_lhs(node))) * _value(_rhs(node));
	Node const *D = _clone(_rhs(_lhs(node)));
	delete node;

	return div(val(k), D);
}

Node const *Simplify::div_combine_1(Node const *node)
{
	float64_t k = _value(_rhs(_lhs(node))) / _value(_rhs(node));
	Node const *x = _clone(_lhs(_lhs(node)));
	delete node;

	return mul(x, val(k));
}

Node const *Simplify::div_combine_2(Node const *node)
{
	float64_t k = _value(_rhs(node)) / _value(_rhs(_lhs(node)));
	Node const *x = _clone(_lhs(_lhs(node)));
	delete node;

	return mul(x, val(k));
}

Node const *Simplify::collapse_multiplication(Node const *node)
{
	Node const *x = _clone (_lhs(_lhs(node)));
	float64_t   a = _value(_rhs(_lhs(node)));
	float64_t   b = _value(_rhs(node));
	delete node;

	return mul(x, val(a * b));
}

Node const *Simplify::compute_div(Node const *node)
{
	float64_t result = _value(_lhs(node)) / _value(_rhs(node));
	delete node;

	return val(result);
}

Node const *Simplify::compute_sin(Node const *node)
{
	float64_t  theta = _value(_arg(node));
	float64_t result = ::sin(theta);
	delete node;

	return val(result);
}

Node const *Simplify::compute_cos(Node const *node)
{
	float64_t  theta = _value(_arg(node));
	float64_t result = ::cos(theta);
	delete node;

	return val(result);
}

Node const *Simplify::compute_exp(Node const *node)
{
	float64_t      x = _value(_arg(node));
	float64_t result = ::exp(x);
	delete node;

	return val(result);
}

Node const *Simplify::compute_log(Node const *node)
{
	float64_t      x = _value(_arg(node));
	float64_t result = ::log(x);
	delete node;

	return val(result);
}

Node const *Simplify::compute_pow(Node const *node)
{
	float64_t      x = _value(_lhs(node));
	float64_t      y = _value(_rhs(node));
	float64_t result = ::pow(x, y);
	delete node;

	return val(result);
}

Node const *Simplify::collapse_identity(Node const *node)
{
	Node const *copied = _clone(_arg(_arg(node)));
	delete node;

	return copied;
}

Node const *Simplify::div_cancel(Node const *node)
{
	if (!_equal(_lhs(node), _rhs(node)))
		return node;
	delete node;

	return val(1);
}

Node const *Simplify::div_pow_cancel(Node const *node)
{
	Node const *x = _lhs(_lhs(node));
	Node const *y = _rhs(node);

	if (!_equal(x, y))
		return node;

	Node const *e = _rhs(_lhs(node));
	Node const *canceled = pow(_clone(x), sub(_clone(e), val(1)));
	delete node;

	return canceled;
}

Node const *Simplify::ddx_function(Node const *node)
{
	/// TBD: ???
	return _clone(node);
}

LUCID_XPR_END