#pragma once

#include <cassert>
#include <vector>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Node.h>

LUCID_XPR_BEGIN

///	Iterator 
///
/// a simple in-order traversal
class Iterator : public Algorithm
{
public:
	Iterator() = default;

	Iterator(Iterator const &) = default;

	explicit Iterator(Node const *node);

	explicit operator Node const *() const;

	virtual ~Iterator() = default;

	Iterator &operator=(Iterator const &) = default;

	Iterator &operator=(Node const *node);

	Iterator &operator++();

	Iterator operator++(int);

	virtual void evaluate(Any const *node) override;

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
	Node const *_current = nullptr;
	std::vector<Node const *> _stack;

	template<typename T> Node const *lhs(T const *node);

	template<typename T> Node const *rhs(T const *node);

	void evaluate_leaf(Node const *node);

	void evaluate_unary(UnaryOperation const *node);

	void evaluate_binary(BinaryOperation const *node);

	void set(Node const *node);

};

inline Iterator::operator Node const *() const
{
	return _current;
}

inline Iterator &Iterator::operator=(Node const *node)
{
	set(node);
	return *this;
}

inline Iterator &Iterator::operator++()
{
	_current->apply(this);
	return *this;
}

inline void Iterator::evaluate(Any const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Constant const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Variable const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Negate const *node)
{
	evaluate_unary(node);
}

inline void Iterator::evaluate(NaturalLogarithm const *node)
{
	evaluate_unary(node);
}

inline void Iterator::evaluate(Sine const *node)
{
	evaluate_unary(node);
}

inline void Iterator::evaluate(Cosine const *node)
{
	evaluate_unary(node);
}

inline void Iterator::evaluate(Add const *node)
{
	evaluate_binary(node);
}

inline void Iterator::evaluate(Subtract const *node)
{
	evaluate_binary(node);
}

inline void Iterator::evaluate(Multiply const *node)
{
	evaluate_binary(node);
}

inline void Iterator::evaluate(Divide const *node)
{
	evaluate_binary(node);
}

inline void Iterator::evaluate(Power const *node)
{
	evaluate_binary(node);
}

template<typename T> inline Node const *Iterator::lhs(T const *node)
{
	assert(nullptr != node->lhs);
	return node->lhs;
}

template<typename T> inline Node const *Iterator::rhs(T const *node)
{
	assert(nullptr != node->rhs);
	return node->rhs;
}

inline void Iterator::evaluate_leaf(Node const *node)
{
	assert(0 != _stack.size());

	_current = _stack.back();
	_stack.pop_back();
}

inline void Iterator::evaluate_unary(UnaryOperation const *node)
{
	_current = rhs(node);
}

inline void Iterator::evaluate_binary(BinaryOperation const *node)
{
	_current = lhs(node);
	_stack.push_back(rhs(node));
}

inline void Iterator::set(Node const *node)
{
	_stack.clear();
	_stack.push_back(nullptr);

	_current = node;
}

LUCID_XPR_END