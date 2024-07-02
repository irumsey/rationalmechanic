#include "StarCatalog.h"
#include "Utility.h"
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>

LUCID_ORBIT_BEGIN

void StarCatalog::initialize(std::string const &path)
{
	LUCID_CORE::Reader &reader = LUCID_CORE::FileReader(path);

	size_t count = reader.read<int32_t>();

	_ordinal.resize(count);
	for (size_t i = 0; i < count; ++i)
	{
		Entry &entry = _ordinal[i];

		entry.            xno = reader.read<int32_t>();
		entry.           type = reader.read<std::string>();
		entry.right_ascension = reader.read<float64_t>();
		entry.    declination = reader.read<float64_t>();
		entry.      magnitude = reader.read<float32_t>();
	}

	core::log("INFO", "Star Catalog initialized using: " + path);
}

void StarCatalog::shutdown()
{
	_ordinal.clear();
}

LUCID_ORBIT_END