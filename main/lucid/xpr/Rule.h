#pragma once

#include <vector>
#include <lucid/xpr/Defines.h>
#include <lucid/xpr/Hasher.h>
#include <lucid/xpr/Node.h>
#include <lucid/xpr/Iterator.h>

LUCID_XPR_BEGIN

///	Rule
///
/// 
template<typename ACT> struct Rule final
{
	typedef typename ACT action_t;

	std::vector<Token> pattern;
	action_t action = nullptr;

	Rule() = delete;

	Rule(Rule const &) = default;

	Rule(std::vector<Token> const &pattern, action_t action)
		: pattern(pattern)
		, action(action)
	{
	}

	Rule &operator=(Rule const &) = default;

	~Rule() = default;

	bool matches(Node const *node) const
	{
		thread_local static Hasher hash;

		std::vector<Token>::const_iterator lhs = pattern.begin();
		Iterator rhs = node;

		while ((pattern.end() != lhs) && (nullptr != rhs))
		{
			if (Token::ANY == *lhs)
			{
				++lhs;
				rhs.skip();
				continue;
			}

			if (*lhs != hash(rhs.ptr()))
			{
				return false;
			}

			++lhs;
			++rhs;
		}

		return ((pattern.end() == lhs) && (nullptr == rhs));
	}

};

LUCID_XPR_END
