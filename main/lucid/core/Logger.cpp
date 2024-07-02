#include "Logger.h"
#include <algorithm>

LUCID_CORE_BEGIN

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::addListener(Listener *listener)
{
	removeListener(listener);
	_listeners.push_back(listener);
}

void Logger::removeListener(Listener *listener)
{
	_listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}

void Logger::initialize()
{
	///	NOP
}

void Logger::shutdown()
{
	_listeners.clear();
}

Logger &Logger::instance()
{
	static Logger theInstance;
	return theInstance;
}

///
///
///

FileLog::FileLog(std::string const &path)
{
	file.open(path.c_str());
}

FileLog::~FileLog()
{
}

void FileLog::onMessage(std::string const &category, std::string const &message)
{
	if (!file.is_open())
	{
		return;
	}
	file << category << ": " << message << std::endl;
}

LUCID_CORE_END