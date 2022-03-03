#include "Filereader.h"
#include "Identity.h"
#include "Error.h"

///
///
///
namespace lucid {
namespace core {

	FileReader::FileReader(std::string const &path)
	{
		LUCID_VALIDATE(open(path), "unable to open file: " + path);
	}

	FileReader::~FileReader()
	{
		close();
	}

	bool FileReader::open(std::string const &path)
	{
		close();
		_file.open(path.c_str(), std::ios::binary);

		return _file.is_open();
	}

	void FileReader::read(void *data, size_t size)
	{
		LUCID_VALIDATE(is_open(), "attempt to read from an unopened file");
		_file.read((char *)data, size);
	}

	void FileReader::read(bool &data)
	{
		read(&data, sizeof(bool));
	}

	void FileReader::read(int8_t &data)
	{
		read(&data, sizeof(int8_t));
	}

	void FileReader::read(uint8_t &data)
	{
		read(&data, sizeof(uint8_t));
	}

	void FileReader::read(int16_t &data)
	{
		read(&data, sizeof(int16_t));
	}

	void FileReader::read(uint16_t &data)
	{
		read(&data, sizeof(uint16_t));
	}

	void FileReader::read(int32_t &data)
	{
		read(&data, sizeof(int32_t));
	}

	void FileReader::read(uint32_t &data)
	{
		read(&data, sizeof(uint32_t));
	}

	void FileReader::read(int64_t &data)
	{
		read(&data, sizeof(int64_t));
	}

	void FileReader::read(uint64_t &data)
	{
		read(&data, sizeof(uint64_t));
	}

	void FileReader::read(float32_t &data)
	{
		read(&data, sizeof(float32_t));
	}

	void FileReader::read(float64_t &data)
	{
		read(&data, sizeof(float64_t));
	}

	void FileReader::read(std::string &data)
	{
		int size = 0;
		read(size);

		data.resize(size);
		if (size) read((void*)&data[0], size);
	}

	void FileReader::read(Identity &data)
	{
		read(data.data(), Identity::SIZE);
	}

}	///	core
}	///	lucid