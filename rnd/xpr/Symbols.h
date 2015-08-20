#pragma once

#include <string>
#include <hash_map>

namespace rnd
{
	namespace xpr
	{
		
		///	Symbols
		///
		///	contains assignments for variables in an expression.
		///
		///	SEE ALSO: Evaluate
		typedef std::hash_map<std::string, double> Symbols;

	}	///	xpr
}		///	rnd