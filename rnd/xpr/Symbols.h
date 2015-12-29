#pragma once

#include <string>
#include <unordered_map>

namespace rnd
{
	namespace xpr
	{
		
		///	Symbols
		///
		///	contains assignments for variables in an expression.
		///
		///	SEE ALSO: Evaluate
		typedef std::unordered_map<std::string, double> Symbols;

	}	///	xpr
}		///	rnd