#pragma once

#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Hasher.h>

LUCID_XPR_BEGIN

class Node;

///	Rule
///
/// 
template<typename T> struct Rule final
{
	typedef Node const *(*Action)(T const *);
	static Node const *nop(T const *node) { return node; };

	Pattern pattern;
	Action action = nop;

	Rule() = delete;

	Rule(Rule const &) = default;

	Rule(Pattern const &pattern, Action action)
		: pattern(pattern)
		, action(action)
	{
	}

	~Rule() = default;

};

LUCID_XPR_END
