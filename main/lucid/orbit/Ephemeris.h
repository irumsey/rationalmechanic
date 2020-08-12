#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>

///
///
///

namespace lucid {
namespace orbit {


	///
	///
	///
	struct Elements
	{
		fixed256_t       eccentricity;
		fixed256_t          periapsis;
		fixed256_t        inclination;
		fixed256_t ascendingLongitude;
		fixed256_t  perifocusArgument;
		fixed256_t      timePeriapsis;
		fixed256_t         meanMotion;
		fixed256_t        meanAnomaly;
		fixed256_t        trueAnomaly;
		fixed256_t      semimajorAxis;
		fixed256_t   apoapsisDistance;
		fixed256_t             period;
	};

	///
	///
	///
	class Ephemeris final
	{
	public:
		virtual ~Ephemeris();

		void initialize(std::string const &path);

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:

		LUCID_PREVENT_COPY(Ephemeris);
		LUCID_PREVENT_ASSIGNMENT(Ephemeris);
	};

}	///	orbit
}	///	lucid
