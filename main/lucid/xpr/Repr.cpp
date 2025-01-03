#include "Repr.h"
#include "Node.h"
#include <sstream>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

Repr::Repr(Node const *node)
{
	repr(node);
}

std::string const &Repr::operator()(Node const *node)
{
	return repr(node);
}

std::string const &Repr::repr(Node const *node)
{
	result.clear();
	node->apply(this);
	
	return result;
}

void Repr::evaluate(Constant const *node)
{
	std::ostringstream stream;
	stream << "val[" << node->value << "]";
	result += stream.str();
}

void Repr::evaluate(Variable const *node)
{
	std::ostringstream stream;
	stream << "var[" << node->index << "]";
	result += stream.str();
}

void Repr::evaluate(Function const *node)
{
	std::ostringstream stream;

	stream << "Fn[" << node->index << "](";
	for (auto const i : node->signature) { stream << i << ","; }

	result += stream.str();
	result.back() = ')';
}

void Repr::evaluate(Derivative const *node)
{
	std::ostringstream stream;
	stream << "D_" << node->index;
	evaluateOperation(stream.str(), node);
}

void Repr::evaluate(Negate const *node)
{
	evaluateOperation("neg", node);
}

void Repr::evaluate(Add const *node)
{
	evaluateOperation("add", node);
}

void Repr::evaluate(Subtract const *node)
{
	evaluateOperation("sub", node);
}

void Repr::evaluate(Multiply const *node)
{
	evaluateOperation("mul", node);
}

void Repr::evaluate(Divide const *node)
{
	evaluateOperation("div", node);
}

void Repr::evaluate(Sine const *node)
{
	evaluateOperation("sin", node);
}

void Repr::evaluate(Cosine const *node)
{
	evaluateOperation("cos", node);
}

void Repr::evaluate(Exponential const *node)
{
	evaluateOperation("exp", node);
}

void Repr::evaluate(Logarithm const *node)
{
	evaluateOperation("log", node);
}

void Repr::evaluate(Power const *node)
{
	evaluateOperation("pow", node);
}

void Repr::evaluateOperation(std::string const &label, UnaryOperation const *oper)
{
	result += label;
	result += "(";
	
	oper->rhs->apply(this);
	
	result += ")";
}

void Repr::evaluateOperation(std::string const &label, BinaryOperation const *oper)
{
	result += label;
	result += "(";

	oper->lhs->apply(this);
	
	result += ",";

	oper->rhs->apply(this);
	
	result += ")";
}

LUCID_XPR_END