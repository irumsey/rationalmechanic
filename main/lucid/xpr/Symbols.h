#pragma once

#include <string>
#include <unordered_map>

namespace lucid {
namespace xpr {
		
	///	Symbols
	///
	///	contains assignments for variables in an expression.
	///
	///	SEE ALSO: Evaluate
	typedef std::unordered_map<std::string, double> Symbols;

}	///	xpr
}	///	lucid