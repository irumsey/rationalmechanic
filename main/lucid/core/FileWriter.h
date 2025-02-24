#pragma once

#include <fstream>
#include <string>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Writer.h>

LUCID_CORE_BEGIN

///	FileWriter
///
///	binary file writer implementation of Writer.
class FileWriter : public Writer
{
public:
	FileWriter() = default;

	FileWriter(std::string const &path);

	virtual ~FileWriter();

	bool is_open() const;

	bool open(std::string const &path);

	void close();

	virtual void write(void const *data, size_t size) override;

	template<typename T> void write(T const &value);

private:
	std::ofstream _file;

	LUCID_PREVENT_COPY(FileWriter);
	LUCID_PREVENT_ASSIGNMENT(FileWriter);
};

inline bool FileWriter::is_open() const
{
	return _file.is_open();
}

inline void FileWriter::close()
{
	if (_file.is_open())
		_file.close();
}

template<typename T> inline void FileWriter::write(T const &value)
{
	Writer::write<T>(value);
}

LUCID_CORE_END