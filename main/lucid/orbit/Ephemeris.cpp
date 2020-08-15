#include "Ephemeris.h"
#include <lucid/math/Algorithm.h>
#include <lucid/core/FileReader.h>

namespace core = ::lucid::core;
namespace math = ::lucid::math;

///
///
///

namespace lucid {
namespace orbit {

	Ephemeris::Ephemeris()
	{
	}

	Ephemeris::~Ephemeris()
	{
	}

	void Ephemeris::initialize(std::string const &path)
	{
		core::FileReader reader(path);

		int32_t bodyCount = 0;
		reader.read(bodyCount);
		for (int32_t bodyIndex = 0; bodyIndex < bodyCount; ++bodyIndex)
		{
			std::string target;
			reader.read(target);

			Entry entry;
			reader.read(entry.center);

			int32_t elementsCount = 0;
			reader.read(elementsCount);

			LUCID_VALIDATE(elementsCount > 1, "target '" + target + "' does not define more than one set of orbital elements");

			for (int32_t elementsIndex = 0; elementsIndex < elementsCount; ++elementsIndex)
			{
				Elements elements;
				reader.read(&elements, sizeof(elements));
				entry.elements.push_back(elements);
			}

			_entries.insert(std::make_pair(target, entry));
		}
	}

	void Ephemeris::lookup(Elements &elements, std::string const &target, float32_t jdn) const
	{
		auto iter = _entries.find(target);
		LUCID_VALIDATE(iter != _entries.end(), "unknown target '" + target + "' specified");

		Entry const &entry = iter->second;

		size_t const count = entry.elements.size();
		size_t const firstIndex = 0;
		size_t const lastIndex = count - 1;

		///	sanity check (should have been discovered during initialization)
		LUCID_VALIDATE(count > 1, "target '" + target + "' does not define more than one set of orbital elements");

		if (jdn < entry.elements[firstIndex].JDN)
		{
			interpolate(elements, jdn, entry.elements[firstIndex], entry.elements[firstIndex + 1]);
		}
		else if (entry.elements[lastIndex].JDN <= jdn)
		{
			interpolate(elements, jdn, entry.elements[lastIndex - 1], entry.elements[lastIndex]);
		}
		else
		{
			bool found = false;
			
			for (size_t index = firstIndex; index < lastIndex; ++index)
			{
				Elements const & first = entry.elements[index + 0];
				Elements const &second = entry.elements[index + 1];

				if ((first.JDN <= jdn) && (jdn < second.JDN))
				{
					interpolate(elements, jdn, first, second);
					found = true;
					break;
				}
			}

			LUCID_VALIDATE(found, "unable to interpolate orbital elements for target '" + target + "'");
		}
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

	void Ephemeris::interpolate(Elements &result, float32_t jdn, Elements const &a, Elements const &b) const
	{
		float32_t u = (jdn - a.JDN) / (b.JDN - a.JDN);

		result.JDN = jdn;

		result.EC = math::interp(u, a.EC, b.EC);
		result.QR = math::interp(u, a.QR, b.QR);
		result.IN = math::interp(u, a.IN, b.IN);
		result.OM = math::interp(u, a.OM, b.OM);
		result. W = math::interp(u, a. W, b. W);
		result.Tp = math::interp(u, a.Tp, b.Tp);
		result. N = math::interp(u, a. N, b. N);
		result.MA = math::interp(u, a.MA, b.MA);
		result.TA = math::interp(u, a.TA, b.TA);
		result. A = math::interp(u, a. A, b. A);
		result.AD = math::interp(u, a.AD, b.AD);
		result.PR = math::interp(u, a.PR, b.PR);
	}

}	///	orbit
}	///	lucid
