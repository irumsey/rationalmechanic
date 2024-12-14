#include "Repr.h"
#include "Node.h"

LUCID_ANONYMOUS_BEGIN

float64_t const TOL = 1e-20;
float64_t const TOLSQ = TOL * TOL;

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

std::string const &Repr::operator()(Node const *node)
{
	result.clear();
	node->apply(this);
	return result;
}

void Repr::evaluate(Any const *node)
{
	result += "*";
}

void Repr::evaluate(Constant const *node)
{
	float64_t diffZero = 0.0 - node->value;
	float64_t diffOne = 1.0 - node->value;

	if ((diffZero * diffZero) < TOLSQ)
	{
		result += "val(0)";
	}
	else if ((diffOne * diffOne) < TOLSQ)
	{
		result += "val(1)";
	}
	else
	{
		result += "val(k)";
	}
}

void Repr::evaluate(Variable const *node)
{
	result += "var(";
	result += node->symbol;
	result += ")";
}

void Repr::evaluate(Negate const *node)
{
	evaluateOperation("neg", node);
}

void Repr::evaluate(NaturalLogarithm const *node)
{
	evaluateOperation("ln", node);
}

void Repr::evaluate(Sine const *node)
{
	evaluateOperation("sin", node);
}

void Repr::evaluate(Cosine const *node)
{
	evaluateOperation("cos", node);
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