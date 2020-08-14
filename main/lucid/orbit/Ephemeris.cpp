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

		reader.read(_epoch);

		int32_t count = 0;
		reader.read(count);

		for (int32_t i = 0; i < count; ++i)
		{
			std::string name;
			reader.read(name);

			Elements elements;
			reader.read(elements.reference);
			reader.read(elements.     semimajorAxis, 2 * sizeof(float32_t));
			reader.read(elements.      eccentricity, 2 * sizeof(float32_t));
			reader.read(elements.       inclination, 2 * sizeof(float32_t));
			reader.read(elements.     meanLongitude, 2 * sizeof(float32_t));
			reader.read(elements.periapsisLongitude, 2 * sizeof(float32_t));
			reader.read(elements.ascendingLongitude, 2 * sizeof(float32_t));

			LUCID_VALIDATE(_map.end() == _map.find(name), "duplidate frame, " + name + ", specified in ephemeris");

			_map.insert(std::make_pair(name, elements));
		}
	}

	Elements Ephemeris::lookup(float32_t date, std::string const &frame) const
	{
		float32_t t = (date - _epoch) / 36525.f;

		auto iter = _map.find(frame);
		LUCID_VALIDATE(iter != _map.end(), "unknown frame," + frame + ", specified");

		Elements const &entry = iter->second;
		Elements result;

		result.     semimajorAxis[0] = entry.     semimajorAxis[0] + t * entry.     semimajorAxis[1];
		result.      eccentricity[0] = entry.      eccentricity[0] + t * entry.      eccentricity[1];
		result.       inclination[0] = entry.       inclination[0] + t * entry.       inclination[1];
		result.     meanLongitude[0] = entry.     meanLongitude[0] + t * entry.     meanLongitude[1];
		result.ascendingLongitude[0] = entry.ascendingLongitude[0] + t * entry.ascendingLongitude[1];
		result.periapsisLongitude[0] = entry.periapsisLongitude[0] + t * entry.periapsisLongitude[1];

		return result;
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

}	///	orbit
}	///	lucid