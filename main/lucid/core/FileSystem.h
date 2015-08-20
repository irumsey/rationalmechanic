#pragma once

#include <cassert>
#include <string>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

///
///
///
namespace lucid {
namespace core {

	///	FileTime
	///
	///
	struct FileTime
	{
		uint32_t highPart, lowPart;

		FileTime(uint32_t highPart = 0, uint32_t lowPart = 0)
			: highPart(highPart), lowPart(lowPart)
		{
		}

		bool operator<(FileTime const &rhs)
		{
			return (highPart < rhs.highPart) || ((highPart == rhs.highPart) && (lowPart < rhs.lowPart));
		}

		bool operator==(FileTime const &rhs)
		{
			return (highPart == rhs.highPart) || (lowPart == rhs.lowPart);
		}
	};

	///	FileInfo
	///
	///
	struct FileInfo
	{
		std::string path;
		FileTime timeWrite;

		FileInfo(std::string const &path = std::string(), FileTime const &timeWrite = FileTime())
			: path(path)
			, timeWrite(timeWrite)
		{
		}

		bool operator<(FileInfo const &rhs) const
		{
			return path < rhs.path;
		}

		bool operator==(FileInfo const &rhs) const
		{
			return path == rhs.path;
		}

		bool operator<(std::string const &rhs)
		{
			return path < rhs;
		}

		bool operator==(std::string const &rhs)
		{
			return path == rhs;
		}
	};

	///	FileSystem
	///
	///	note: file system does not take ownership of the listener.
	class FileSystem
	{
	public:
		///	Listener
		///
		///
		class Listener
		{
		public:
			virtual ~Listener() = 0 {}

			virtual void onFileModified(FileInfo const &info) = 0;

		protected:
			Listener() {}

		};

		virtual ~FileSystem() = default;

		virtual void addListener(std::string const &path, Listener *listener) = 0;

		virtual void removeListener(Listener *listener) = 0;

		virtual void scan() = 0;

		virtual void initialize() = 0;

		virtual void shutdown() = 0;

		static FileSystem &instance();

	protected:
		FileSystem() = default;

		LUCID_PREVENT_COPY(FileSystem);
		LUCID_PREVENT_ASSIGNMENT(FileSystem);
	};

	///	FileDelegate
	///
	///	Note: use is optional
	template<class T> class FileDelegate : public FileSystem::Listener
	{
	public:
		FileDelegate() = default;

		FileDelegate(std::string const &path, T *listener)
		{
			enable(path, listener);
		}

		virtual ~FileDelegate()
		{
			disable();
		}

		void enable(std::string const &path, T *listener)
		{
			FileSystem &fileSystem = FileSystem::instance();
			fileSystem.addListener(path, this);

			_path = path;
			_listener = listener;
		}

		void disable()
		{
			FileSystem &fileSystem = FileSystem::instance();
			fileSystem.removeListener(this);

			_path.clear();
			_listener = nullptr;
		}

		void onFileModified(FileInfo const &info)
		{
			assert(_path == info.path);
			if (_listener)
			{
				_listener->onFileModified(info);
			}
		}

	private:
		std::string _path;
		T *_listener = nullptr;

	};

}	///	core
}	///	lucid