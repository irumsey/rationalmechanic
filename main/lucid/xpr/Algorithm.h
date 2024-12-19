#pragma once

#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

class Constant;
class Variable;
class Negate;
class Sine;
class Cosine;
class Add;
class Subtract;
class Multiply;
class Divide;
class Exponential;
class Logarithm;
class Power;

///	Algorithm
///
///	a "visitor" to be applied to an expression node.
///
///	NOTE: this is pure abstract.  note the lack of "helper"
///	methods for traversal.  each algorithm might require: pre-order,
///	in-order, and/or post-order traversals.  some might not traverse
///	a tree at all, simply acting on any one given node.  assumptions
///	concerning future algorithms will not be made here.
///
///	SEE ALSO: Node
class Algorithm abstract
{
public:
	virtual ~Algorithm() = default;

	virtual void evaluate(Constant const *node) = 0;

	virtual void evaluate(Variable const *node) = 0;

	virtual void evaluate(Negate const *node) = 0;

	virtual void evaluate(Add const *node) = 0;

	virtual void evaluate(Subtract const *node) = 0;

	virtual void evaluate(Multiply const *node) = 0;

	virtual void evaluate(Divide const *node) = 0;

	virtual void evaluate(Sine const *node) = 0;

	virtual void evaluate(Cosine const *node) = 0;

	virtual void evaluate(Exponential const *node) = 0;

	virtual void evaluate(Logarithm const *node) = 0;

	virtual void evaluate(Power const *node) = 0;

protected:
	Algorithm() = default;

};

LUCID_XPR_END