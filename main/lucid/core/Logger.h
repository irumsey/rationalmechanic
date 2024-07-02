#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>

LUCID_CORE_BEGIN

///	Logger
///
///	logging facility.
class Logger
{
public:
	///	Listener
	///
	///	subject/observer callback pattern.
	class Listener
	{
	public:
		virtual ~Listener() = 0 {}

		virtual void onMessage(std::string const &category, std::string const &message) = 0;

	protected:
		Listener() {}

	};

	///	Callback
	///
	///	callback implementation of Listener.
	class Callback : public Listener
	{
	public:
		typedef void(*func_type)(std::string const &category, std::string const &message);

		Callback(func_type func)
			: func(func)
		{
		}

		virtual void onMessage(std::string const &category, std::string const &message) override
		{
			func(category, message);
		}

	private:
		func_type func = nullptr;
	};

	virtual ~Logger();

	void addListener(Listener *listener);

	void removeListener(Listener *listener);

	void log(std::string const &category, std::string const &message);

	void initialize();

	void shutdown();

	static Logger &instance();

protected:
	Logger();

private:
	std::vector<Listener*> _listeners;

	LUCID_PREVENT_COPY(Logger);
	LUCID_PREVENT_ASSIGNMENT(Logger);
};

inline void Logger::log(std::string const &category, std::string const &message)
{
	for (auto listener : _listeners) { listener->onMessage(category, message); }
}

///	FileLog
///
///	specific listener which writes to a file.
class FileLog : public Logger::Listener
{
public:
	FileLog(std::string const &path);

	virtual ~FileLog();

	virtual void onMessage(std::string const &category, std::string const &message);

private:
	std::ofstream file;

};

///	log
///
///	global convienience function for logging messages.
///	got tired of getting instance first.
inline void log(std::string const &category, std::string const &message)
{
	Logger &logger = Logger::instance();
	logger.log(category, message);
}

LUCID_CORE_END