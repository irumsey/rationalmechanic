#pragma once

#include <vector>
#include <unordered_map>
#include <lucid/core/Defines.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>
#include <rnd/_orbit/Elements.h>
#include <rnd/_orbit/Dictionary.h>

LUCID_CORE_BEGIN

class Reader;
class Writer;

LUCID_CORE_END

ORBIT_BEGIN

/// Ephemeris
/// 
/// 
class Ephemeris
{
public:
	Ephemeris() = default;

	Ephemeris(Dictionary const &dictionary, LUCID_CORE::Reader &reader);

	virtual ~Ephemeris() = default;

	OrbitalElements const &lookup(Frame const *frame, scalar_t jdn) const;

	void add(Frame const *frame, OrbitalElements const &elements);

	void read(Dictionary const &dictionary, LUCID_CORE::Reader &reader);

	void clear();

private:
	typedef std::vector<OrbitalElements> series_t;
	typedef std::pair<bool, series_t> entry_t;
	typedef std::unordered_map<Frame const *, entry_t> entries_t;

	// Making it mutable so the series can be sorted if required.
	// It is guranteed that, other than sorting wrt JDN, the series
	// data will remain un-altered.
	mutable entries_t _entries;

};

inline void Ephemeris::add(Frame const *frame, OrbitalElements const &elements)
{
	entry_t &entry = _entries[frame];

	entry.first = true;
	entry.second.push_back(elements);
}

inline void Ephemeris::clear()
{
	_entries.clear();
}

ORBIT_END
