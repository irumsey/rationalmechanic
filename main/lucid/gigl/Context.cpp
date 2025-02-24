#include "Context.h"
#include "Resources.h"
#include <lucid/core/Unserializer.h>

LUCID_GIGL_BEGIN

Context::Context(std::string const &path)
{
	initialize(LUCID_CORE::Unserializer(path));
}

Context::Context(LUCID_CORE::Unserializer &reader)
{
	initialize(reader);
}

void Context::initialize(LUCID_CORE::Unserializer &reader)
{
	_values.clear();

	int32_t count = reader.read<int32_t>();
	for (int32_t i = 0; i < count; ++i)
	{
		reader.nested_begin();

		std::string name = reader.read<std::string>();
		add(name, Primitive(reader));

		reader.nested_end();
	}
}

LUCID_GIGL_END