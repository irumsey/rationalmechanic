#include "FileWriter.h"
#include "Identity.h"
#include "Error.h"

///
///
///
namespace lucid {
namespace core {

	FileWriter::FileWriter(std::string const &path)
	{
		open(path);
	}

	FileWriter::~FileWriter()
	{
		close();
	}

	bool FileWriter::open(std::string const &path)
	{
		close();
		_file.open(path.c_str(), std::ios::binary);
		return _file.is_open();
	}

	void FileWriter::write(void const *data, int size)
	{
		LUCID_VALIDATE(isOpen(), "attempt to write to an unopened file");
		_file.write((char const *)data, size);
	}

	void FileWriter::write(bool data)
	{
		write(&data, sizeof(bool));
	}

	void FileWriter::write(int8_t data)
	{
		write(&data, sizeof(int8_t));
	}

	void FileWriter::write(uint8_t data)
	{
		write(&data, sizeof(uint8_t));
	}

	void FileWriter::write(int16_t data)
	{
		write(&data, sizeof(int16_t));
	}

	void FileWriter::write(uint16_t data)
	{
		write(&data, sizeof(uint16_t));
	}

	void FileWriter::write(int32_t data)
	{
		write(&data, sizeof(int32_t));
	}

	void FileWriter::write(uint32_t data)
	{
		write(&data, sizeof(uint32_t));
	}

	void FileWriter::write(int64_t data)
	{
		write(&data, sizeof(int64_t));
	}

	void FileWriter::write(uint64_t data)
	{
		write(&data, sizeof(uint64_t));
	}

	void FileWriter::write(float32_t data)
	{
		write(&data, sizeof(float32_t));
	}

	void FileWriter::write(float64_t data)
	{
		write(&data, sizeof(float64_t));
	}

	void FileWriter::write(char const *data)
	{
		int size = (int) ::strlen(data);
		write(size);
		if (size) write(data, size);
	}

	void FileWriter::write(std::string const &data)
	{
		int size = (int)data.size();
		write(size);
		if (size) write(&data[0], size);
	}

	void FileWriter::write(Identity const &data)
	{
		write(data.data(), Identity::SIZE);
	}

}	///	core
}	///	lucid