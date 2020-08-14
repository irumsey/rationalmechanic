#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>

///
///
///

namespace lucid {
namespace orbit {


	///	Orbital elements
	///
	///
	struct Elements
	{
		std::string            reference;
		float32_t       semimajorAxis[2];
		float32_t        eccentricity[2];
		float32_t         inclination[2];
		float32_t       meanLongitude[2];
		float32_t  periapsisLongitude[2];
		float32_t  ascendingLongitude[2];
	};

	///	Ephemeris
	///
	///	
	class Ephemeris final
	{
	public:
		virtual ~Ephemeris();

		void initialize(std::string const &path);

		Elements lookup(float32_t date, std::string const &frame) const;

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:
		typedef std::unordered_map<std::string, Elements> map_t;

		float32_t _epoch = 0.f;
		map_t _map;

		void interpolate(Elements &point, Elements const *ctrl, fixed256_t const &u) const;

		LUCID_PREVENT_COPY(Ephemeris);
		LUCID_PREVENT_ASSIGNMENT(Ephemeris);
	};

}	///	orbit
}	///	lucid
