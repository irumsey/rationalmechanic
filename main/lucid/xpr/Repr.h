#pragma once

#include <ostream>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;

///	Repr
///
///	creates a "representation" of the expression.
///	uses the supplied std::ostream as output.  keeps
///	the stream as reference, therefore keep an eye on
///	scope.
///
///	NOTE: Repr is NOT for pretty printing, it is a
///	"utility" for use in simplification (and debugging).
///
///	SEE ALSO: Simplify
class Repr : public Algorithm
{
public:
	Repr(std::ostream &stream);

	virtual ~Repr() = default;

	void operator()(Node const *node);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(NaturalLogarithm const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Power const *node) override;

private:
	std::ostream &stream;

	void evaluateOperation(std::string const &label, UnaryOperation const *oper);

	void evaluateOperation(std::string const &label, BinaryOperation const *oper);

};

LUCID_XPR_END