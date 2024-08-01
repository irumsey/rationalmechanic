#pragma once

#include <lucid/core/Logger.h>
#include <lucid/math/Vector.h>

///
/// 
/// 

typedef LUCID_MATH::Vector<int32_t, 2> point2d_t;

///
///
///

inline void log(std::string const &category, std::string const &message)
{
	lucid::core::Logger &logger = lucid::core::Logger::instance();

	logger.log(category, message);
}

///
///
///

void dumpProfileData(std::string const &path);