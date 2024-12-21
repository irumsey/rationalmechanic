#pragma once

#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Hasher.h>
#include <lucid/xpr/Action.h>

LUCID_XPR_BEGIN

class Node;

///	Rule
///
/// 
struct Rule final
{
	std::vector<Token> pattern;
	action_type action = action::nop;

	Rule() = delete;

	Rule(Rule const &) = default;

	Rule(std::vector<Token> const &pattern, action_type action)
		: pattern(pattern)
		, action(action)
	{
	}

	~Rule() = default;

};

LUCID_XPR_END
