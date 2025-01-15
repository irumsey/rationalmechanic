#pragma once

#include <string>
#include <ostream>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;
class Registry;

///	LaTeX
///
/// 
class LaTeX : public Algorithm
{
public:
	LaTeX() = default;

	LaTeX(Node const *node, Registry const &registry);

	virtual ~LaTeX() = default;

	std::string const &operator()(Node const *node, Registry const &registry);

	std::string const &format(Node const *node, Registry const &registry);

	std::string const &str() const;

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

	virtual void evaluate(Function const *node) override;

	virtual void evaluate(Derivative const *node) override;

	virtual void evaluate(Negate const *node) override;

	virtual void evaluate(Add const *node) override;

	virtual void evaluate(Subtract const *node) override;

	virtual void evaluate(Multiply const *node) override;

	virtual void evaluate(Divide const *node) override;

	virtual void evaluate(Sine const *node) override;

	virtual void evaluate(Cosine const *node) override;

	virtual void evaluate(Exponential const *node) override;

	virtual void evaluate(Logarithm const *node) override;

private:
	Registry const *symbols = nullptr;
	std::string result;

	void evaluateOperation(std::string const &label, UnaryOperation const *oper);

	void evaluateOperation(std::string const &label, BinaryOperation const *oper);

};

inline std::string const &LaTeX::str() const
{
	return result;
}

LUCID_XPR_END

inline std::ostream &operator<<(std::ostream &stream, LUCID_XPR::LaTeX const &repr)
{
	stream << repr.str();
	return stream;
}
