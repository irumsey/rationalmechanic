#pragma once

#include <cstdint>
#include <exception>
#include <string>
#include <lucid/core/Defines.h>

///
///
///

#define LUCID_THROW(msg) throw LUCID_CORE::Error( __FILE__ , __LINE__ , msg )
#define LUCID_VALIDATE(xpr, msg) if(!(xpr)) LUCID_THROW(msg)

LUCID_CORE_BEGIN

///	Error
///
///	exception thrown by the lucid system.
class Error : public std::exception
{
public:
	Error() = default;

	Error(std::string const &file, int32_t line, std::string const &error);

	virtual ~Error() = default;

	char const *whatFile() const;

	int32_t whatLine() const;

	char const *whatError() const;

	char const *what() const;

private:
	std::string _file = "<unknown>";
	int32_t _line = 0;
	std::string _error = "<unknown>";
	std::string _message = "<unknown>";

};

inline char const *Error::whatFile() const
{
	return _file.c_str();
}

inline int32_t Error::whatLine() const
{
	return _line;
}

inline char const *Error::whatError() const
{
	return _error.c_str();
}

inline char const *Error::what() const
{
	return _message.c_str();
}

LUCID_CORE_END