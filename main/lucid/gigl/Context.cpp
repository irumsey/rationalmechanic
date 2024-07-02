#include "Context.h"
#include "Resources.h"
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>

LUCID_GIGL_BEGIN

Context::Context(std::string const &path)
{
	initialize(::lucid::core::FileReader(path));
}

Context::Context(::lucid::core::Reader &reader)
{
	initialize(reader);
}

void Context::initialize(::lucid::core::Reader &reader)
{
	_values.clear();

	int32_t count = reader.read<int32_t>();
	for (int32_t i = 0; i < count; ++i)
	{
		std::string name = reader.read<std::string>();
		add(name, Primitive(reader));
	}
}

LUCID_GIGL_END