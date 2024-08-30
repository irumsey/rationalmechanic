#pragma once

#include <string>
#include <vector>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Defines.h>

#define LUCID_ORBIT_STARCATALOG LUCID_ORBIT::StarCatalog::instance()

LUCID_ORBIT_BEGIN

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

	typedef std::vector<Entry> ordinal_vec_t;
	typedef ordinal_vec_t::const_iterator Iterator;

	virtual ~StarCatalog() = default;

	void initialize(std::string const &path);

	void shutdown();

	size_t count() const;

	Entry const &operator[](size_t index) const;

	Iterator begin() const;

	Iterator end() const;

	static StarCatalog &instance();

protected:
	StarCatalog() = default;

private:
	ordinal_vec_t _ordinal;

	LUCID_PREVENT_COPY(StarCatalog);
	LUCID_PREVENT_ASSIGNMENT(StarCatalog);
};

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

inline StarCatalog &StarCatalog::instance()
{
	static StarCatalog theInstance;
	return theInstance;
}

LUCID_ORBIT_END
