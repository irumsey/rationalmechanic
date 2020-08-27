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
namespace core {

	class Reader;

}	///	core
}	/// lucid

///
///
///

namespace lucid {
namespace orbit {

	struct Properties;
	struct Elements;
	class  Frame;

	///	Ephemeris
	///
	///	
	class Ephemeris final
	{
	public:
		typedef std::vector<std::string> ordinal_vec_t;
		typedef ordinal_vec_t::const_iterator Iterator;

		///
		///
		///
		struct Entry
		{
			enum TYPE
			{
				TYPE_UNDEFINED     = 0,
				TYPE_DYNAMIC_POINT = 1,
				TYPE_ORBITAL_BODY  = 2,
				TYPE_DYNAMIC_BODY  = 3,
			};

			TYPE  type = TYPE_UNDEFINED;
			size_t  id = 0;
			size_t cid = 0;
			std::string name;
			std::string description;
		};

		virtual ~Ephemeris();

		void initialize(std::string const &path);
		
		Iterator begin() const;

		Iterator end() const;

		bool lookup(Entry &entry, std::string const &target) const;
			 			 
		bool lookup(Properties &properties, std::string const &target) const;
			 
		bool lookup(Properties &properties, size_t target) const;
			 
		bool lookup(Elements &elements, std::string const &target, scalar_t jdn) const;
			 
		bool lookup(Elements &elements, size_t target, scalar_t jdn) const;

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:
		typedef std::vector<Elements> elements_vec_t;

		typedef std::unordered_map<std::string, Entry> entry_map_t;
		typedef std::unordered_map<size_t, Properties> properties_map_t;
		typedef std::unordered_map<size_t, elements_vec_t> elements_map_t;

		ordinal_vec_t _order;
		entry_map_t _entries;
		properties_map_t _properties;
		elements_map_t _elements;

		LUCID_PREVENT_COPY(Ephemeris);
		LUCID_PREVENT_ASSIGNMENT(Ephemeris);
	};

	inline Ephemeris::Iterator Ephemeris::begin() const
	{
		return _order.begin();
	}

	inline Ephemeris::Iterator Ephemeris::end() const
	{
		return _order.end();
	}

	inline bool Ephemeris::lookup(Entry &entry, std::string const &target) const
	{
		auto iter = _entries.find(target);
		if (iter == _entries.end())
			return false;

		entry = iter->second;

		return true;
	}

	inline bool Ephemeris::lookup(Properties &properties, std::string const &target) const
	{
		auto iter = _entries.find(target);
		if (iter == _entries.end())
			return false;

		Entry const &entry = iter->second;

		return lookup(properties, entry.id);
	}

	inline bool Ephemeris::lookup(Elements &elements, std::string const &target, scalar_t jdn) const
	{
		auto iter = _entries.find(target);
		if (iter == _entries.end())
			return false;

		Entry const &entry = iter->second;

		return lookup(elements, entry.id, jdn);
	}

}	///	orbit
}	///	lucid
