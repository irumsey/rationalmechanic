#include "StarCatalog.h"
#include "Utility.h"
#include <lucid/gal/Types.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>
#include <unordered_map>

LUCID_ANONYMOUS_BEGIN

std::unordered_map<std::string, LUCID_GAL::Color> colors;

inline void colorInitialize()
{
	colors["O"] = LUCID_GAL::Color(157.f / 255.f, 180.f / 255.f, 255.f / 255.f, 1.f);
	colors["B"] = LUCID_GAL::Color(170.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.f);
	colors["A"] = LUCID_GAL::Color(202.f / 255.f, 216.f / 255.f, 255.f / 255.f, 1.f);
	colors["F"] = LUCID_GAL::Color(251.f / 255.f, 248.f / 255.f, 255.f / 255.f, 1.f);
	colors["G"] = LUCID_GAL::Color(255.f / 255.f, 244.f / 255.f, 232.f / 255.f, 1.f);
	colors["K"] = LUCID_GAL::Color(255.f / 255.f, 215.f / 255.f, 174.f / 255.f, 1.f);
	colors["M"] = LUCID_GAL::Color(255.f / 255.f, 190.f / 255.f, 127.f / 255.f, 1.f);
	colors["R"] = LUCID_GAL::Color(255.f / 255.f,  80.f / 255.f,  80.f / 255.f, 1.f);
	colors["N"] = LUCID_GAL::Color(255.f / 255.f,  60.f / 255.f,  60.f / 255.f, 1.f);

	colors["O0"] = LUCID_GAL::Color(155.f / 255.f, 176.f / 255.f, 255.f / 255.f, 1.f);
	colors["O5"] = LUCID_GAL::Color(157.f / 255.f, 180.f / 255.f, 255.f / 255.f, 1.f);
	colors["B0"] = LUCID_GAL::Color(160.f / 255.f, 185.f / 255.f, 255.f / 255.f, 1.f);
	colors["B5"] = LUCID_GAL::Color(170.f / 255.f, 191.f / 255.f, 255.f / 255.f, 1.f);
	colors["A0"] = LUCID_GAL::Color(200.f / 255.f, 215.f / 255.f, 255.f / 255.f, 1.f);
	colors["A5"] = LUCID_GAL::Color(202.f / 255.f, 216.f / 255.f, 255.f / 255.f, 1.f);
	colors["F0"] = LUCID_GAL::Color(248.f / 255.f, 247.f / 255.f, 255.f / 255.f, 1.f);
	colors["F5"] = LUCID_GAL::Color(251.f / 255.f, 248.f / 255.f, 255.f / 255.f, 1.f);
	colors["G0"] = LUCID_GAL::Color(255.f / 255.f, 244.f / 255.f, 234.f / 255.f, 1.f);
	colors["G5"] = LUCID_GAL::Color(255.f / 255.f, 244.f / 255.f, 232.f / 255.f, 1.f);
	colors["K0"] = LUCID_GAL::Color(255.f / 255.f, 235.f / 255.f, 209.f / 255.f, 1.f);
	colors["K5"] = LUCID_GAL::Color(255.f / 255.f, 215.f / 255.f, 174.f / 255.f, 1.f);
	colors["M0"] = LUCID_GAL::Color(255.f / 255.f, 204.f / 255.f, 111.f / 255.f, 1.f);
	colors["M5"] = LUCID_GAL::Color(255.f / 255.f, 190.f / 255.f, 127.f / 255.f, 1.f);
	colors["R0"] = LUCID_GAL::Color(255.f / 255.f, 100.f / 255.f, 100.f / 255.f, 1.f);
	colors["R5"] = LUCID_GAL::Color(255.f / 255.f, 80.f / 255.f, 80.f / 255.f, 1.f);
	colors["N0"] = LUCID_GAL::Color(255.f / 255.f, 70.f / 255.f, 70.f / 255.f, 1.f);
	colors["N5"] = LUCID_GAL::Color(255.f / 255.f, 60.f / 255.f, 60.f / 255.f, 1.f);
}

inline LUCID_GAL::Color lookupColor(std::string const &type)
{
	static LUCID_GAL::Color const colorDefault(1, 1, 1, 1);

	auto iter = colors.find(type);
	if (iter != colors.end())
		return iter->second;

	iter = colors.find(type.substr(0, 1));
	if (iter != colors.end())
		return iter->second;

	LUCID_CORE::log("WARN", "unknown star type, " + type + ", specified");
	return colorDefault;
}

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

void StarCatalog::initialize(std::string const &path)
{
	LUCID_CORE::Reader &reader = LUCID_CORE::FileReader(path);

	colorInitialize();

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
		entry.          color = lookupColor(entry.type);
	}

	LUCID_CORE::log("INFO", "Star Catalog initialized using: " + path);
}

void StarCatalog::shutdown()
{
	_ordinal.clear();
}

LUCID_ORBIT_END
