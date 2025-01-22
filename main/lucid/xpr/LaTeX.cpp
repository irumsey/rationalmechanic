#include "LaTeX.h"
#include "Registry.h"
#include "Node.h"

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

///
///
/// 

LaTeX::LaTeX()
{
	initialize();
}

LaTeX::LaTeX(Node const *node, Registry const &registry)
{
	initialize();
	format(node, registry);
}

std::string LaTeX::operator()(Node const *node, Registry const &registry)
{
	return format(node, registry);
}

std::string LaTeX::format(Node const *node, Registry const &registry)
{
	symbols = &registry;

	stream.str("");
	stream.clear();

	stream << "\\[";
	node->apply(this);
	stream << "\\]";

	return str();
}

void LaTeX::evaluate(Constant const *node)
{
	stream << node->value;
}

void LaTeX::evaluate(Variable const *node)
{
	stream << symbols->symbol_for(node->index);
}

void LaTeX::evaluate(Function const *node)
{
	Function::Signature const &signature = node->signature;

	stream << "\\operatorname{" << symbols->symbol_for(node->index) << "}(";
	for (size_t i = 0; i < signature.size(); ++i)
	{
		stream << symbols->symbol_for(signature[i]);
		if (i < (signature.size() - 1)) { stream << ", "; }
	}
	stream << ")";
}

void LaTeX::evaluate(Derivative const *node)
{
	applyRules(node);
}

void LaTeX::evaluate(Negate const *node)
{
	applyRules(node);
}

void LaTeX::evaluate(Add const *node)
{
	_lhs(node)->apply(this);
	stream << " + ";
	_rhs(node)->apply(this);
}

void LaTeX::evaluate(Subtract const *node)
{
	_lhs(node)->apply(this);
	stream << " - ";
	_rhs(node)->apply(this);
}

void LaTeX::evaluate(Multiply const *node)
{
	applyRules(node);
}

void LaTeX::evaluate(Divide const *node)
{
	stream << "\\frac{";
	_lhs(node)->apply(this);
	stream << "}{";
	_rhs(node)->apply(this);
	stream << "}";
}

void LaTeX::evaluate(Sine const *node)
{
	stream << "sin(";
	_arg(node)->apply(this);
	stream << ")";
}

void LaTeX::evaluate(Cosine const *node)
{
	stream << "cos(";
	_arg(node)->apply(this);
	stream << ")";
}

void LaTeX::evaluate(Exponential const *node)
{
	stream << "e^{";
	_arg(node)->apply(this);
	stream << "}";
}

void LaTeX::evaluate(Logarithm const *node)
{
	stream << "log(";
	_arg(node)->apply(this);
	stream << ")";
}

void LaTeX::initialize()
{
	addRule(rule_t( { Token::NEG, Token::ADD, Token::ANY, Token::ANY, }, &LaTeX::format_neg));
	addRule(rule_t( { Token::NEG, Token::SUB, Token::ANY, Token::ANY, }, &LaTeX::format_neg));
	addRule(rule_t( { Token::NEG, Token::ANY,                         }, &LaTeX::format_neg_default));
	
	addRule(rule_t( { Token::DDX, Token::ADD, Token::ANY, Token::ANY, }, &LaTeX::format_ddx));
	addRule(rule_t( { Token::DDX, Token::SUB, Token::ANY, Token::ANY, }, &LaTeX::format_ddx));
	addRule(rule_t( { Token::DDX, Token::ANY,                         }, &LaTeX::format_ddx_default));

	addRule(rule_t( { Token::MUL, Token::ADD, Token::ANY, Token::ANY, Token::ADD, Token::ANY, Token::ANY, }, &LaTeX::format_mul_0));
	addRule(rule_t( { Token::MUL, Token::ADD, Token::ANY, Token::ANY, Token::SUB, Token::ANY, Token::ANY, }, &LaTeX::format_mul_0));
	addRule(rule_t( { Token::MUL, Token::SUB, Token::ANY, Token::ANY, Token::ADD, Token::ANY, Token::ANY, }, &LaTeX::format_mul_0));
	addRule(rule_t( { Token::MUL, Token::SUB, Token::ANY, Token::ANY, Token::SUB, Token::ANY, Token::ANY, }, &LaTeX::format_mul_0));
	addRule(rule_t( { Token::MUL, Token::ADD, Token::ANY, Token::ANY, Token::ANY, }, &LaTeX::format_mul_1));
	addRule(rule_t( { Token::MUL, Token::SUB, Token::ANY, Token::ANY, Token::ANY, }, &LaTeX::format_mul_1));
	addRule(rule_t( { Token::MUL, Token::ANY, Token::ADD, Token::ANY, Token::ANY, }, &LaTeX::format_mul_2));
	addRule(rule_t( { Token::MUL, Token::ANY, Token::SUB, Token::ANY, Token::ANY, }, &LaTeX::format_mul_2));
	addRule(rule_t( { Token::MUL, Token::ANY, Token::ANY,                         }, &LaTeX::format_mul_default));
}

///
/// 
/// 

void LaTeX::format_ddx(Node const *node)
{
	stream << "\\frac{d}{d" << symbols->symbol_for(_index(node)) << "}(";
	_arg(node)->apply(this);
	stream << ")";
}

void LaTeX::format_ddx_default(Node const *node)
{
	stream << "\\frac{d}{d" << symbols->symbol_for(_index(node)) << "}";
	_arg(node)->apply(this);
}

void LaTeX::format_neg(Node const *node)
{
	stream << "-(";
	_arg(node)->apply(this);
	stream << ")";
}

void LaTeX::format_neg_default(Node const *node)
{
	stream << "-";
	_arg(node)->apply(this);
}

void LaTeX::format_mul_0(Node const *node)
{
	stream << "(";
	_lhs(node)->apply(this);
	stream << ") \\cdot (";
	_rhs(node)->apply(this);
	stream << ")";
}

void LaTeX::format_mul_1(Node const *node)
{
	stream << "(";
	_lhs(node)->apply(this);
	stream << ") \\cdot ";
	_rhs(node)->apply(this);
}

void LaTeX::format_mul_2(Node const *node)
{
	_lhs(node)->apply(this);
	stream << "\\cdot (";
	_rhs(node)->apply(this);
	stream << ")";
}

void LaTeX::format_mul_default(Node const *node)
{
	_lhs(node)->apply(this);
	stream << " \\cdot ";
	_rhs(node)->apply(this);
}

LUCID_XPR_END