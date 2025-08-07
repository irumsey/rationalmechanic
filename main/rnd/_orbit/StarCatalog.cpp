#include "StarCatalog.h"
#include <lucid/core/Unserializer.h>
#include <lucid/core/Logger.h>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

ORBIT_BEGIN

StarCatalog::StarCatalog(LUCID_CORE::Unserializer &stream)
{
	initialize(stream);
}

void StarCatalog::initialize(LUCID_CORE::Unserializer &stream)
{
	clear();

	colorInitialize(stream);

	size_t count = stream.read<int32_t>();
	_ordinal.resize(count);

	for (size_t i = 0; i < count; ++i)
	{
		Entry &entry = _ordinal[i];

		stream.nested_begin();
		entry.            xno = stream.read<int32_t>();
		entry.           type = stream.read<std::string>();
		entry.right_ascension = stream.read<float64_t>();
		entry.    declination = stream.read<float64_t>();
		entry.      magnitude = stream.read<float32_t>();
		entry.          color = lookupColor(entry.type);
		stream.nested_end();
	}
}

void StarCatalog::clear()
{
	_colors.clear();
	_ordinal.clear();
}

void StarCatalog::colorInitialize(LUCID_CORE::Unserializer &stream)
{
	_colors.clear();

	size_t count = stream.read<int32_t>();
	for (size_t i = 0; i < count; ++i)
	{
		stream.nested_begin();
		std::string type = stream.read<std::string>();
		LUCID_VALIDATE(4 == stream.read<int32_t>(), "expecting 4-tuple color");
		LUCID_GAL::Color color = stream.read<LUCID_GAL::Color>();
		stream.nested_end();

		LUCID_VALIDATE(_colors.end() == _colors.find(type), "duplicate star type color specified");
		_colors[type] = color;
	}
}

ORBIT_END
