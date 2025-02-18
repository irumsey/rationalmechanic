#include "FileWriter.h"
#include "Identity.h"
#include "Error.h"
 
LUCID_CORE_BEGIN

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

void FileWriter::write(void const *data, size_t size)
{
	LUCID_VALIDATE(is_open(), "attempt to write to an unopened file");
	_file.write((char const *)data, size);
}

LUCID_CORE_END