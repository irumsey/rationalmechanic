#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>

namespace lucid {
namespace xpr
{

	class Algorithm;

	///	Node
	///
	///	base for a node in an expression tree.  the
	///	nodes supply the structure of the expression, the algorithms
	///	supply the behavior.
	///
	///	SEE ALSO: Algorithm
	class Node
	{
	public:
		virtual ~Node() = default;

		virtual void apply(Algorithm *algorithm) const = 0;

	protected:
		Node() = default;

		LUCID_PREVENT_COPY(Node);
		LUCID_PREVENT_ASSIGNMENT(Node);
	};

	///	Constant
	///
	///
	class Constant : public Node
	{
	public:
		double const value;

		Constant(double const &value);

		virtual ~Constant() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Constant);
		LUCID_PREVENT_ASSIGNMENT(Constant);
	};

	inline Node const *val(double value)
	{
		return new Constant(value);
	}

	///	Variable
	///
	///
	class Variable : public Node
	{
	public:
		std::string const symbol;

		Variable(std::string const &symbol);

		virtual ~Variable() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Variable);
		LUCID_PREVENT_ASSIGNMENT(Variable);
	};

	inline Node const *var(std::string const &symbol)
	{
		return new Variable(symbol);
	}

	///	UnaryOperation
	///
	///	"function" with one input (ie an operation with one child).
	class UnaryOperation : public Node
	{
	public:
		Node const * const rhs = nullptr;

		virtual ~UnaryOperation();

	protected:
		UnaryOperation(Node const *rhs);

		LUCID_PREVENT_COPY(UnaryOperation);
		LUCID_PREVENT_ASSIGNMENT(UnaryOperation);
	};

	///	BinaryOperation
	///
	///	"function" with two inputs (ie an operation with two children).
	class BinaryOperation : public Node
	{
	public:
		Node const * const lhs = nullptr;
		Node const * const rhs = nullptr;

		virtual ~BinaryOperation();

	protected:
		BinaryOperation(Node const *lhs, Node const *rhs);

		LUCID_PREVENT_COPY(BinaryOperation);
		LUCID_PREVENT_ASSIGNMENT(BinaryOperation);
	};

	///	Negate
	///
	///
	class Negate : public UnaryOperation
	{
	public:
		Negate(Node const *rhs);

		virtual ~Negate() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Negate);
		LUCID_PREVENT_ASSIGNMENT(Negate);
	};

	inline Node const *neg(Node const *rhs)
	{
		return new Negate(rhs);
	}

	///	NaturalLogarithm
	///
	///
	class NaturalLogarithm : public UnaryOperation
	{
	public:
		NaturalLogarithm(Node const *rhs);

		virtual ~NaturalLogarithm() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(NaturalLogarithm);
		LUCID_PREVENT_ASSIGNMENT(NaturalLogarithm);
	};

	inline Node const *ln(Node const *rhs)
	{
		return new NaturalLogarithm(rhs);
	}

	///	Sine
	///
	///
	class Sine : public UnaryOperation
	{
	public:
		Sine(Node const *rhs);

		virtual ~Sine() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Sine);
		LUCID_PREVENT_ASSIGNMENT(Sine);
	};

	inline Node const *sin(Node const *rhs)
	{
		return new Sine(rhs);
	}

	///	Cosine
	///
	///
	class Cosine : public UnaryOperation
	{
	public:
		Cosine(Node const *rhs);

		virtual ~Cosine() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Cosine);
		LUCID_PREVENT_ASSIGNMENT(Cosine);
	};

	inline Node const *cos(Node const *rhs)
	{
		return new Cosine(rhs);
	}

	///	Add
	///
	///
	class Add : public BinaryOperation
	{
	public:
		Add(Node const *lhs, Node const *rhs);

		virtual ~Add() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Add);
		LUCID_PREVENT_ASSIGNMENT(Add);
	};

	inline Node const *add(Node const *lhs, Node const *rhs)
	{
		return new Add(lhs, rhs);
	}

	///	Subtract
	///
	///
	class Subtract : public BinaryOperation
	{
	public:
		Subtract(Node const *lhs, Node const *rhs);

		virtual ~Subtract() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Subtract);
		LUCID_PREVENT_ASSIGNMENT(Subtract);
	};

	inline Node const *sub(Node const *lhs, Node const *rhs)
	{
		return new Subtract(lhs, rhs);
	}

	///	Multiply
	///
	///
	class Multiply : public BinaryOperation
	{
	public:
		Multiply(Node const *lhs, Node const *rhs);

		virtual ~Multiply() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Multiply);
		LUCID_PREVENT_ASSIGNMENT(Multiply);
	};

	inline Node const *mul(Node const *lhs, Node const *rhs)
	{
		return new Multiply(lhs, rhs);
	}

	///	Divide
	///
	///
	class Divide : public BinaryOperation
	{
	public:
		Divide(Node const *lhs, Node const *rhs);

		virtual ~Divide() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Divide);
		LUCID_PREVENT_ASSIGNMENT(Divide);
	};

	inline Node const *div(Node const *lhs, Node const *rhs)
	{
		return new Divide(lhs, rhs);
	}

	///	Power
	///
	///
	class Power : public BinaryOperation
	{
	public:
		Power(Node const *lhs, Node const *rhs);

		virtual ~Power() = default;

		virtual void apply(Algorithm *algorithm) const override;

		LUCID_PREVENT_COPY(Power);
		LUCID_PREVENT_ASSIGNMENT(Power);
	};

	inline Node const *pow(Node const *lhs, Node const *rhs)
	{
		return new Power(lhs, rhs);
	}

}	///	xpr
}	/// lucid