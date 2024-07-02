#pragma once

#include <cassert>
#include <string>
 
template<typename T> inline T cast(char const *)
{
	static_assert(false, "bad cast of string type");
	return T();
}

template<typename T> inline T cast(std::string const &)
{
	static_assert(false, "bad cast of string type");
	return T();
}
