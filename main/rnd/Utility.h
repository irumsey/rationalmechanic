#pragma once

#include <string>
#include <lucid/core/Types.h>
#include <lucid/core/Logger.h>
#include <lucid/math/Vector.h>

///
/// 
/// 

enum MOUSE_BUTTON
{	
	MOUSE_BUTTON_LEFT = 0,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT,
};

///
/// 
/// 

typedef LUCID_MATH::Vector<int32_t, 2> point2d_t;

///
///
///

inline void log(std::string const &category, std::string const &message)
{
	LUCID_CORE::Logger &logger = LUCID_CORE::Logger::instance();
	logger.log(category, message);
}

inline void logInfo(std::string const &message)
{
	log("INFO", message);
}

inline void logWarn(std::string const &message)
{
	log("WARN", message);
}

inline void logError(std::string const &message)
{
	log(" ERR", message);
}

///
///
///

void dumpProfileData(std::string const &path);