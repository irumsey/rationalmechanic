#include "FileSystem.h"
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <algorithm>
#include <windows.h>

///
///
///

namespace core = ::lucid::core;

///
///
///
namespace lucid {
namespace core {

	FileSystem &FileSystem::instance()
	{
		return ::lucid::core::win::FileSystem::instance();
	}

}	///	core
}	///	lucid

///
///
///
namespace lucid {
namespace core {
namespace win {

	void FileSystem::addListener(std::string const &path, Listener *listener)
	{
		auto iter = _listeners.find(path);
		if (iter != _listeners.end())
		{
			std::list<Listener*> &lst = iter->second;
			lst.push_back(listener);
		}
		else
		{
			WIN32_FILE_ATTRIBUTE_DATA data;
			if (::GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data))
			{
				FileTime timeWrite(data.ftLastWriteTime.dwHighDateTime, data.ftLastWriteTime.dwLowDateTime);
				FileInfo info(path, timeWrite);

				std::list<Listener*> lst;
				lst.push_back(listener);

				_listeners[info] = lst;
			}
			else
			{
				core::Logger &logger = core::Logger::instance();
				logger.log("ERROR", "unable to add file listener for '" + path + "'");
			}
		}
	}

	void FileSystem::removeListener(Listener *listener)
	{
		auto iter = _listeners.begin();
		while (iter != _listeners.end())
		{
			std::list<Listener*> &lst = iter->second;

			lst.erase(std::remove(lst.begin(), lst.end(), listener), lst.end());

			if (lst.empty())
			{
				iter = _listeners.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	struct Notify
	{
		FileInfo const &info;

		Notify(FileInfo const &info) :
			info(info)
		{
		}

		inline void operator()(FileSystem::Listener *listener) const
		{
			listener->onFileModified(info);
		}
	};

	void FileSystem::scan()
	{
		///	TBD: if the number of watched files grows too large, amortize this over multiple frames.
		auto iter = _listeners.begin();
		for (; iter != _listeners.end(); iter++)
		{
			///	the const_cast<> is ugly but not dangerous.
			///	we are not altering that which is defined
			///	unique for _listeners functionality.
			FileInfo &info = const_cast<FileInfo&>(iter->first);
			std::list<Listener*> &lst = iter->second;

			WIN32_FILE_ATTRIBUTE_DATA data;
			if (::GetFileAttributesExA(info.path.c_str(), GetFileExInfoStandard, &data))
			{
				FileTime timeWrite(data.ftLastWriteTime.dwHighDateTime, data.ftLastWriteTime.dwLowDateTime);
				if (info.timeWrite < timeWrite)
				{
					info.timeWrite = timeWrite;
					std::for_each(lst.begin(), lst.end(), Notify(info));
				}
			}
			else
			{
				core::Logger &logger = core::Logger::instance();
				logger.log("ERROR", "unable to update file info for '" + info.path + "'");
			}
		}
	}

	void FileSystem::initialize()
	{
		///	NOP
	}

	void FileSystem::shutdown()
	{
		_listeners.clear();
	}

}	///	win
}	///	core
}	///	lucid
