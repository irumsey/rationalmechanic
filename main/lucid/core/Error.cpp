#include "Error.h"
#include <sstream>

///
///
///
namespace lucid {
namespace core {

	Error::Error(std::string const &file, int32_t line, std::string const &error)
		: _file(file)
		, _line(line)
		, _error(error)
	{
		std::ostringstream os;
		os << file << "(" << line << "): " << error;
		_message = os.str();
	}

}	/// core
}	/// lucid