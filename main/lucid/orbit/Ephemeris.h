#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>

///
///
///

namespace lucid {
namespace orbit {

	struct Properties;
	struct Elements;

	///	Ephemeris
	///
	///	
	class Ephemeris final
	{
	public:
		virtual ~Ephemeris();

		void   initialize(std::string const &path);

		size_t lookup(std::string const &target) const;

		void   lookup(Properties &properties, std::string const &target) const;

		void   lookup(Properties &properties, size_t target) const;

		size_t lookup(Elements &elements, std::string const &target, scalar_t jdn) const;

		size_t lookup(Elements &elements, size_t target, scalar_t jdn) const;

		void   compute(matrix3x3_t &rotation, std::string const &target, scalar_t jdn) const;

		void   compute(matrix3x3_t &rotation, size_t target, scalar_t jdn) const;

		void   compute(matrix3x3_t &rotation, Elements const &elements) const;

		void   compute(vector3_t &position, vector3_t &velocity, std::string const &target, scalar_t jdn) const;
			   
		void   compute(vector3_t &position, vector3_t &velocity, size_t target, scalar_t jdn) const;

		void   compute(vector3_t &position, vector3_t &velocity, Properties const &centerProperties, Elements const &targetElements, scalar_t jdn) const;

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:
		typedef std::vector<Elements> elements_vec_t;

		struct Entry
		{
			size_t center = 0;
			elements_vec_t elements;
		};

		typedef std::unordered_map<std::string, size_t> name_map_t;
		typedef std::unordered_map<size_t, Properties> properties_map_t;
		typedef std::unordered_map<size_t, Entry> entry_map_t;

		name_map_t _names;
		properties_map_t _properties;
		entry_map_t _entries;

		LUCID_PREVENT_COPY(Ephemeris);
		LUCID_PREVENT_ASSIGNMENT(Ephemeris);
	};


}	///	orbit
}	///	lucid
