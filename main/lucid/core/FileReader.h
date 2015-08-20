#pragma once

#include <fstream>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Reader.h>

///
///
///
namespace lucid {
namespace core {

	///	FileReader
	///
	///	binary file reading implementation of Reader.
	class FileReader : public Reader
	{
	public:
		FileReader() = default;

		FileReader(std::string const &path);

		virtual ~FileReader();

		bool isOpen() const;

		bool open(std::string const &path);

		void close();

		virtual void read(void *data, int size) override;

		virtual void read(bool &data) override;

		virtual void read(int8_t &data) override;

		virtual void read(uint8_t &data) override;

		virtual void read(int16_t &data) override;

		virtual void read(uint16_t &data) override;

		virtual void read(int32_t &data) override;

		virtual void read(uint32_t &data) override;

		virtual void read(int64_t &data) override;

		virtual void read(uint64_t &data) override;

		virtual void read(float32_t &data) override;

		virtual void read(float64_t &data) override;

		virtual void read(std::string &data) override;

		virtual void read(Identity &data) override;

	private:
		std::ifstream _file;

		LUCID_PREVENT_COPY(FileReader);
		LUCID_PREVENT_ASSIGNMENT(FileReader);
	};

	inline bool FileReader::isOpen() const
	{
		return _file.is_open();
	}

	inline void FileReader::close()
	{
		if (_file.is_open())
			_file.close();
	}

}	///	core
}	///	lucid