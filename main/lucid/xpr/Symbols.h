#pragma once

#include <string>
#include <unordered_map>
#include <lucid/xpr/Defines.h>

LUCID_XPR_BEGIN

///	Symbols
///
///	contains assignments for variables in an expression.
///
///	SEE ALSO: Evaluate
typedef std::unordered_map<std::string, float64_t> Symbols;

LUCID_XPR_END