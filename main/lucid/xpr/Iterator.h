#pragma once

#include <cassert>
#include <vector>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>
#include <lucid/xpr/Node.h>

LUCID_XPR_BEGIN

///	Iterator 
///
/// a simple pre-order traversal used by the simplification
/// system.  introduces the concept of a "skip subtree" method 
/// which is used to implement a "wildcard" in pattern matching.
/// 
/// See : Simplify
class Iterator : public Algorithm
{
public:
	Iterator() = default;

	Iterator(Iterator const &) = default;

	Iterator(Node const *node);

	virtual ~Iterator() = default;

	Iterator &operator=(Iterator const &) = default;

	Iterator &operator=(Node const *node);

	Iterator &operator++();

	Iterator operator++(int);

	Node const &operator*() const;

	Node const *operator->() const;

	Node const &ref() const;

	Node const *ptr() const;

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

	virtual void evaluate(Power const *node) override;

	void skip();

	bool equ(Node const *node) const;

	bool neq(Node const *node) const;

private:
	std::vector<Node const *> _stack;
	Node const *_current = nullptr;

	template<typename T> Node const *lhs(T const *node);

	template<typename T> Node const *rhs(T const *node);

	void evaluate_leaf(Node const *node);

	void evaluateOperation(UnaryOperation const *node);

	void evaluateOperation(BinaryOperation const *node);

	void set(Node const *node);

	void pop();

};

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

inline Iterator Iterator::operator++(int)
{
	Iterator temp = *this;
	_current->apply(this);
	return temp;
}

inline Node const &Iterator::operator*() const
{
	return ref();
}

inline Node const *Iterator::operator->() const
{
	return ptr();
}

inline Node const &Iterator::ref() const
{
	assert(nullptr != _current);
	return *_current;
}

inline Node const *Iterator::ptr() const
{
	assert(nullptr != _current);
	return _current;
}

inline void Iterator::evaluate(Constant const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Variable const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Function const *node)
{
	evaluate_leaf(node);
}

inline void Iterator::evaluate(Derivative const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Negate const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Add const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Subtract const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Multiply const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Divide const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Sine const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Cosine const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Exponential const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Logarithm const *node)
{
	evaluateOperation(node);
}

inline void Iterator::evaluate(Power const *node)
{
	evaluateOperation(node);
}

inline void Iterator::skip()
{
	pop();
}

inline bool Iterator::equ(Node const *node) const
{
	return node == _current;
}

inline bool Iterator::neq(Node const *node) const
{
	return node != _current;
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
	pop();
}

inline void Iterator::evaluateOperation(UnaryOperation const *node)
{
	_current = rhs(node);
}

inline void Iterator::evaluateOperation(BinaryOperation const *node)
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

inline void Iterator::pop()
{
	assert(0 != _stack.size());

	_current = _stack.back();
	_stack.pop_back();
}

LUCID_XPR_END

inline bool operator==(LUCID_XPR::Node const *lhs, LUCID_XPR::Iterator const &rhs)
{
	return rhs.equ(lhs);
}

inline bool operator==(LUCID_XPR::Iterator const &lhs, LUCID_XPR::Node const *rhs)
{
	return lhs.equ(rhs);
}

inline bool operator!=(LUCID_XPR::Node const *lhs, LUCID_XPR::Iterator const &rhs)
{
	return rhs.neq(lhs);
}

inline bool operator!=(LUCID_XPR::Iterator const &lhs, LUCID_XPR::Node const *rhs)
{
	return lhs.neq(rhs);
}
