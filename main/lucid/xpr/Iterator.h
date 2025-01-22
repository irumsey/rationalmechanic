#pragma once

#include <cassert>
#include <vector>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;
class UnaryOperation;
class BinaryOperation;

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

	void skip();

	bool equ(Node const *node) const;

	bool neq(Node const *node) const;

private:
	std::vector<Node const *> stack;
	Node const *current = nullptr;

	void evaluateLeaf(Node const *node);

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
	assert(nullptr != current);
	return *current;
}

inline Node const *Iterator::ptr() const
{
	assert(nullptr != current);
	return current;
}

inline void Iterator::skip()
{
	pop();
}

inline bool Iterator::equ(Node const *node) const
{
	return node == current;
}

inline bool Iterator::neq(Node const *node) const
{
	return node != current;
}

inline void Iterator::set(Node const *node)
{
	stack.clear();
	stack.push_back(nullptr);

	current = node;
}

inline void Iterator::pop()
{
	assert(0 != stack.size());

	current = stack.back();
	stack.pop_back();
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
