#pragma once

#include <lucid/core/Types.h>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Algorithm.h>

LUCID_XPR_BEGIN

class Node;
class Iterator;

///	TID
/// 
/// See: Hasher
/// See: Simplify
template<typename T> struct TYPE
{
	static size_t ID()
	{
		return size_t(&ID);
	}
};

///	Token
/// 
/// Hashed value of a node.  Contains a unique
/// ID for the node type and a value representing
/// the node's data (if any).
/// 
/// Note: uses unsigned integers for type ids and data.
///       (-1) is a "don't care" or match anything wildcard.
/// 
/// See : Hasher
/// See : Simplify
struct Token final
{
	size_t tid = -1;
	union {
		uint64_t index = -1;
		float64_t value;
	};

	Token() = default;

	Token(size_t tid)
		: tid(tid)
		, index(-1)
	{
	}

	Token(size_t tid, uint64_t index)
		: tid(tid)
		, index(index)
	{
	}

	Token(size_t tid, float64_t value)
		: tid(tid)
		, value(value)
	{
	}

	~Token() = default;

};

///	Hasher
///
/// 
class Hasher : public Algorithm
{
public:
	Hasher() = default;

	virtual ~Hasher() = default;

	Token operator()(Iterator const &iter);

	Token operator()(Node const *node);

	Token hash(Iterator const &iter);

	Token hash(Node const *node);

	virtual void evaluate(Constant const *node) override;

	virtual void evaluate(Variable const *node) override;

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

private:
	Token token;

};

inline Token Hasher::operator()(Iterator const &iter)
{
	return hash(iter);
}

inline Token Hasher::operator()(Node const *node)
{
	return hash(node);
}

LUCID_XPR_END

inline bool operator==(LUCID_XPR::Token const &lhs, LUCID_XPR::Token const &rhs)
{
	return (lhs.tid == rhs.tid) && ((size_t(-1) == lhs.index) || (lhs.index == rhs.index));
}

inline bool operator!=(LUCID_XPR::Token const &lhs, LUCID_XPR::Token const &rhs)
{
	return !(lhs == rhs);
}