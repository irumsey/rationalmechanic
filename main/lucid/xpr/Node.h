#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

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
#if defined(_DEBUG)
	virtual ~Node() { --instances; }
#else
	virtual ~Node() = default;	
#endif

	virtual void apply(Algorithm *algorithm) const = 0;

protected:
#if defined(_DEBUG)
	Node() { ++instances; }
#else
	Node() = default;
#endif

	static size_t instances;

	LUCID_PREVENT_COPY(Node);
	LUCID_PREVENT_ASSIGNMENT(Node);
};

///	Constant
///
///
class Constant : public Node
{
public:
	float64_t const value = 0.0;

	Constant(float64_t const &value);

	virtual ~Constant() = default;

	virtual void apply(Algorithm *algorithm) const override;

	LUCID_PREVENT_COPY(Constant);
	LUCID_PREVENT_ASSIGNMENT(Constant);
};

inline Node const *val(float64_t value)
{
	return new Constant(value);
}

///	Variable
///
///
class Variable : public Node
{
public:
	uint64_t index = -1;

	Variable(uint64_t index);

	virtual ~Variable() = default;

	virtual void apply(Algorithm *algorithm) const override;

	LUCID_PREVENT_COPY(Variable);
	LUCID_PREVENT_ASSIGNMENT(Variable);
};

inline Node const *var(uint64_t index)
{
	return new Variable(index);
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

inline UnaryOperation const *neg(Node const *rhs)
{
	return new Negate(rhs);
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

inline BinaryOperation const *add(Node const *lhs, Node const *rhs)
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

inline BinaryOperation const *sub(Node const *lhs, Node const *rhs)
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

inline BinaryOperation const *mul(Node const *lhs, Node const *rhs)
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

inline BinaryOperation const *div(Node const *lhs, Node const *rhs)
{
	return new Divide(lhs, rhs);
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

inline UnaryOperation const *sin(Node const *rhs)
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

inline UnaryOperation const *cos(Node const *rhs)
{
	return new Cosine(rhs);
}

///	Exponential
///
/// 
class Exponential : public UnaryOperation
{
public:
	Exponential(Node const *rhs);

	virtual ~Exponential() = default;

	virtual void apply(Algorithm *algorithm) const override;

	LUCID_PREVENT_COPY(Exponential);
	LUCID_PREVENT_ASSIGNMENT(Exponential);
};

inline UnaryOperation const *exp(Node const *rhs)
{
	return new Exponential(rhs);
}

///	Logarithm
///
///	natural logarithm
class Logarithm : public UnaryOperation
{
public:
	Logarithm(Node const *rhs);

	virtual ~Logarithm() = default;

	virtual void apply(Algorithm *algorithm) const override;

	LUCID_PREVENT_COPY(Logarithm);
	LUCID_PREVENT_ASSIGNMENT(Logarithm);
};

inline UnaryOperation const *log(Node const *rhs)
{
	return new Logarithm(rhs);
}

///
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

inline BinaryOperation const *pow(Node const *lhs, Node const *rhs)
{
	return new Power(lhs, rhs);
}

/// 
///	derived
/// 

inline BinaryOperation const *tan(Node const *rhs)
{
	return div(sin(rhs), cos(rhs));
}

inline BinaryOperation const *csc(Node const *rhs)
{
	return div(val(1.0), sin(rhs));
}

inline BinaryOperation const *sec(Node const *rhs)
{
	return div(val(1.0), cos(rhs));
}

inline BinaryOperation const *cot(Node const *rhs)
{
	return div(cos(rhs), sin(rhs));
}

LUCID_XPR_END
