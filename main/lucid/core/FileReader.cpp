#include "Filereader.h"
#include "Identity.h"
#include "Error.h"

LUCID_CORE_BEGIN

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

LUCID_CORE_END