#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Elements.h>

///
///
///

namespace lucid {
namespace orbit {

	///	Ephemeris
	///
	///	
	class Ephemeris final
	{
	public:
		virtual ~Ephemeris();

		void initialize(std::string const &path);

		void lookup(Elements &elements, std::string const &target, float32_t jdn) const;

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:
		typedef std::vector<Elements> elements_vec_t;

		struct Entry
		{
			std::string center;
			elements_vec_t elements;
		};

		typedef std::unordered_map<std::string, Entry> entry_map_t;

		entry_map_t _entries;

		void interpolate(Elements &result, float32_t jdn, Elements const &a, Elements const &b) const;

		LUCID_PREVENT_COPY(Ephemeris);
		LUCID_PREVENT_ASSIGNMENT(Ephemeris);
	};

}	///	orbit
}	///	lucid
