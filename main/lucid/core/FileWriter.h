#pragma once

#include <fstream>
#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Writer.h>

///
///
///
namespace lucid {
namespace core {

	///	FileWriter
	///
	///	binary file writer implementation of Writer.
	class FileWriter : public Writer
	{
	public:
		FileWriter() = default;

		FileWriter(std::string const &path);

		virtual ~FileWriter();

		bool isOpen() const;

		bool open(std::string const &path);

		void close();

		virtual void write(void const *data, int size) override;

		virtual void write(bool data) override;

		virtual void write(int8_t data) override;

		virtual void write(uint8_t data) override;

		virtual void write(int16_t data) override;

		virtual void write(uint16_t data) override;

		virtual void write(int32_t data) override;

		virtual void write(uint32_t data) override;

		virtual void write(int64_t data) override;

		virtual void write(uint64_t data) override;

		virtual void write(float32_t data) override;

		virtual void write(float64_t data) override;

		virtual void write(char const *data) override;

		virtual void write(std::string const &data) override;

		virtual void write(Identity const &data) override;

	private:
		std::ofstream _file;

		LUCID_PREVENT_COPY(FileWriter);
		LUCID_PREVENT_ASSIGNMENT(FileWriter);
	};

	inline bool FileWriter::isOpen() const
	{
		return _file.is_open();
	}

	inline void FileWriter::close()
	{
		if (_file.is_open())
			_file.close();
	}

}	///	core
}	///	lucid