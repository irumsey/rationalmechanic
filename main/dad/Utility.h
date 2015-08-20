#pragma once

#include <lucid/core/Logger.h>

///
///
///
inline void log(std::string const &category, std::string const &message)
{
	lucid::core::Logger &logger = lucid::core::Logger::instance();

	logger.log(category, message);
}
