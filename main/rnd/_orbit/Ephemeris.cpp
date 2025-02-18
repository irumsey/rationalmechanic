#include "Ephemeris.h"
#include "Dictionary.h"
#include "Frame.h"
#include <lucid/core/Profiler.h>
#include <lucid/core/Reader.h>
#include <lucid/core/Writer.h>
#include <lucid/core/Error.h>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

ORBIT_BEGIN

Ephemeris::Ephemeris(Dictionary const &dictionary, LUCID_CORE::Reader &reader)
{
	read(dictionary, reader);
}

OrbitalElements const &Ephemeris::lookup(Frame const *frame, scalar_t jdn) const
{
	LUCID_PROFILE_SCOPE("Ephemeris::lookup()");

	auto iter = _entries.find(frame);
	LUCID_VALIDATE(iter != _entries.end(), "invalid frame requested");

	entry_t &entry = iter->second;
	series_t &series = entry.second;

	if (entry.first)
	{
		auto predicate = [](OrbitalElements const &lhs, OrbitalElements const &rhs) { return lhs.JDN < rhs.JDN; };
		std::sort(series.begin(), series.end(), predicate);
		entry.first = false;
	}

	size_t count = series.size();
	LUCID_VALIDATE(count > 1, "insufficient number of entries in series");

	size_t first = 0;
	size_t last = count - 1;
	size_t mid = (last + first) >> 1;

	LUCID_VALIDATE((series[first].JDN <= jdn) && (jdn < series[last].JDN), "specified JDN out of bounds");

	while ((first != mid) && (mid != last))
	{
		if ((series[first].JDN <= jdn) && (jdn < series[mid].JDN))
			last = mid;
		else
			first = mid;
		mid = (last + first) >> 1;
	}

	return series[first];
}

void Ephemeris::write(Dictionary const &dictionary, LUCID_CORE::Writer &writer) const
{
	writer.write<size_t>(_entries.size());

	for (auto iter = _entries.begin(); iter != _entries.end(); ++iter)
	{
		Frame const *frame = iter->first;
		writer.write<std::string>(frame->name);

		entry_t const &entry = iter->second;
		series_t const &series = entry.second;

		LUCID_VALIDATE(1 < series.size(), "insufficient number of entries in series");

		writer.write<size_t>(series.size());
		writer.write(&series[0], series.size() * sizeof(OrbitalElements));
	}
}

void Ephemeris::read(Dictionary const &dictionary, LUCID_CORE::Reader &reader)
{
	_entries.clear();

	size_t entryCount = reader.read<size_t>();
	for (size_t entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		std::string name = reader.read<std::string>();

		Frame const *frame = dictionary[name];
		LUCID_VALIDATE(_entries.end() == _entries.find(frame), "dupliate entry for '" + name + "'");

		size_t elementsCount = reader.read<size_t>();
		LUCID_VALIDATE(1 < elementsCount, "insufficient number of entries in series");

		entry_t &entry = _entries[frame];

		entry.first = true;
		entry.second.resize(elementsCount);

		reader.read(&entry.second[0], elementsCount * sizeof(OrbitalElements));
	}
}

ORBIT_END
