#include "LaTeX.h"
#include "Registry.h"
#include "Node.h"
#include <sstream>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

LaTeX::LaTeX(Node const *node, Registry const &registry)
{
	format(node, registry);
}

std::string const &LaTeX::operator()(Node const *node, Registry const &registry)
{
	return format(node, registry);
}

std::string const &LaTeX::format(Node const *node, Registry const &registry)
{
	symbols = &registry;
	result.clear();

	result += "\\[";
	node->apply(this);
	result += "\\]";

	return result;
}

void LaTeX::evaluate(Constant const *node)
{
	std::ostringstream stream;
	stream << node->value;
	result += stream.str();
}

void LaTeX::evaluate(Variable const *node)
{
	std::ostringstream stream;
	stream << symbols->symbol_for(node->index);
	result += stream.str();
}

void LaTeX::evaluate(Function const *node)
{
	std::ostringstream stream;

	stream << "\\operatorname{" << symbols->symbol_for(node->index) << "}(";
	for (auto const i : node->signature) { stream << symbols->symbol_for(i) << ","; }

	result += stream.str();
	result.back() = ')';
}

void LaTeX::evaluate(Derivative const *node)
{
	std::ostringstream stream;
	stream << "\\frac{d}{d" << symbols->symbol_for(node->index) << "}";
	evaluateOperation(stream.str(), node);
}

void LaTeX::evaluate(Negate const *node)
{
	evaluateOperation("-", node);
}

void LaTeX::evaluate(Add const *node)
{
	evaluateOperation("+", node);
}

void LaTeX::evaluate(Subtract const *node)
{
	evaluateOperation("-", node);
}

void LaTeX::evaluate(Multiply const *node)
{
	evaluateOperation("\\cdot", node);
}

void LaTeX::evaluate(Divide const *node)
{
	result += "\\frac{";
	node->lhs->apply(this);
	result += "}{";
	node->rhs->apply(this);
	result += "}";
}

void LaTeX::evaluate(Sine const *node)
{
	evaluateOperation("\\sin", node);
}

void LaTeX::evaluate(Cosine const *node)
{
	evaluateOperation("\\cos", node);
}

void LaTeX::evaluate(Exponential const *node)
{
	result += "e^{";
	node->rhs->apply(this);
	result += "}";
}

void LaTeX::evaluate(Logarithm const *node)
{
	evaluateOperation("ln", node);
}

void LaTeX::evaluate(Power const *node)
{
	result += "{";
	node->lhs->apply(this);
	result += "}^{";
	node->rhs->apply(this);
	result += "}";
}

void LaTeX::evaluateOperation(std::string const &label, UnaryOperation const *oper)
{
	result += label;
	result += "(";
	oper->rhs->apply(this);
	result += ")";
}

void LaTeX::evaluateOperation(std::string const &label, BinaryOperation const *oper)
{
	result += "(";
	oper->lhs->apply(this);
	result += label;
	oper->rhs->apply(this);
	result += ")";
}

LUCID_XPR_END