#include "Unserializer.h"
#include "FileReader.h"
#include "Error.h"
#include "memory.h"

LUCID_CORE_BEGIN

Unserializer::Unserializer(std::string const &path)
{
	open(path);
}

Unserializer::~Unserializer()
{
	close();
}

bool Unserializer::open(std::string const &path)
{
	close();

	_current = new FileReader(path);
	if (_current->is_open())
		LUCID_VALIDATE(false == read<bool>(), "expecting inline data, not a reference");
	else
		reset_raw_ptr(_current);

	return is_open();
}

void Unserializer::close()
{
	LUCID_VALIDATE(0 == _stack.size(), "serialization consistency error");
	reset_raw_ptr(_current);
}

void Unserializer::read(void *data, size_t size)
{
	LUCID_VALIDATE(is_open(), "unserializer not open");
	_current->read(data, size);
}

void Unserializer::nested_begin(char const *comment)
{
	LUCID_VALIDATE(is_open(), "unserializer not open");

	_stack.push_back(_current);
	if (_current->read<bool>())
	{
		_current = new FileReader(_current->read<std::string>());
		LUCID_VALIDATE(false == read<bool>(), "expecting inline data, not a reference");
	}
}

void Unserializer::nested_end(char const *comment)
{
	LUCID_VALIDATE(is_open(), "unserializer not open");
	LUCID_VALIDATE(0 != _stack.size(), "serialization consistency error");

	FileReader *prev = _stack.back();
	_stack.pop_back();

	if (_current != prev)
		delete _current;
	_current = prev;
}

LUCID_CORE_END
