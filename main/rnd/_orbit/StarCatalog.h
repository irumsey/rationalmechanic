#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

ORBIT_BEGIN

///	StarCatalog
///
///
class StarCatalog
{
public:
	///
	///
	///
	struct Entry
	{
		size_t xno = 0;
		std::string type;
		float64_t right_ascension = 0.0;
		float64_t declination = 0.0;
		float32_t magnitude = 0.f;
		LUCID_GAL::Color color = { 0, 0, 0, 0 };
	};

	typedef std::unordered_map<std::string, LUCID_GAL::Color> color_map_t;

	typedef std::vector<Entry> ordinal_vec_t;
	typedef ordinal_vec_t::const_iterator Iterator;

	StarCatalog() = default;

	StarCatalog(LUCID_CORE::Unserializer &stream);

	virtual ~StarCatalog() = default;

	void initialize(LUCID_CORE::Unserializer &stream);

	void clear();

	LUCID_GAL::Color lookupColor(std::string const &type);

	size_t count() const;

	Entry const &operator[](size_t index) const;

	Iterator begin() const;

	Iterator end() const;

private:
	color_map_t _colors;
	ordinal_vec_t _ordinal;

	void colorInitialize(LUCID_CORE::Unserializer &stream);

	LUCID_PREVENT_COPY(StarCatalog);
	LUCID_PREVENT_ASSIGNMENT(StarCatalog);
};

inline LUCID_GAL::Color StarCatalog::lookupColor(std::string const &type)
{
	static LUCID_GAL::Color const colorDefault(1, 1, 1, 1);

	auto iter = _colors.find(type);
	if (iter != _colors.end())
		return iter->second;

	iter = _colors.find(type.substr(0, 1));
	if (iter != _colors.end())
		return iter->second;

	return colorDefault;
}

inline size_t StarCatalog::count() const
{
	return _ordinal.size();
}

inline StarCatalog::Entry const &StarCatalog::operator[](size_t index) const
{
	return _ordinal[index];
}

inline StarCatalog::Iterator StarCatalog::begin() const
{
	return _ordinal.begin();
}

inline StarCatalog::Iterator StarCatalog::end() const
{
	return _ordinal.end();
}

ORBIT_END
