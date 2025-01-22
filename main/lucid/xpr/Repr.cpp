#include "Repr.h"
#include "Node.h"
#include "Registry.h"

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_XPR_BEGIN

Repr::Repr(Node const *node, Registry const &registry)
{
	repr(node, registry);
}

std::string Repr::operator()(Node const *node, Registry const &registry)
{
	return repr(node, registry);
}

std::string Repr::repr(Node const *node, Registry const &registry)
{
	symbols = &registry;

	stream.str("");
	stream.clear();

	node->apply(this);
	
	return str();
}

void Repr::evaluate(Constant const *node)
{
	stream << "val(" << node->value << ")";
}

void Repr::evaluate(Variable const *node)
{
	stream << "var(" << symbols->symbol_for(node->index) << ")";
}

void Repr::evaluate(Function const *node)
{
	stream << "fn( " << symbols->symbol_for(node->index) << ", { ";
	for (auto const i : node->signature) { stream << "var(" << symbols->symbol_for(i) << "), "; }
	stream << "})";
}

void Repr::evaluate(Derivative const *node)
{
	stream << "ddx( ";
	node->rhs->apply(this);
	stream << ", " << symbols->symbol_for(node->index) << ")";
}

void Repr::evaluate(Negate const *node)
{
	stream << "neg( ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Add const *node)
{
	stream << "add( ";
	node->lhs->apply(this);
	stream << ", ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Subtract const *node)
{
	stream << "sub( ";
	node->lhs->apply(this);
	stream << ", ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Multiply const *node)
{
	stream << "mul( ";
	node->lhs->apply(this);
	stream << ", ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Divide const *node)
{
	stream << "div( ";
	node->lhs->apply(this);
	stream << ", ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Sine const *node)
{
	stream << "sin( ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Cosine const *node)
{
	stream << "cos( ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Exponential const *node)
{
	stream << "exp( ";
	node->rhs->apply(this);
	stream << ")";
}

void Repr::evaluate(Logarithm const *node)
{
	stream << "log( ";
	node->rhs->apply(this);
	stream << ")";
}

LUCID_XPR_END