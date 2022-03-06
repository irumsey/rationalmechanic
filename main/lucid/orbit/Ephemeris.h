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

	struct PhysicalProperties;
	struct RenderProperties;
	struct Elements;

	///	Ephemeris
	///
	///	Note: while camera frame is here for completeness, the ephemeris file shouldn't
	/// define camera frames.
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
			///	the values are defined in the ephemeris file.
			///	defining these to match.
			enum TYPE
			{
				TYPE_UNDEFINED     = 0,
				TYPE_DYNAMIC_POINT = 1,
				TYPE_ORBITAL_BODY  = 2,
				TYPE_DYNAMIC_BODY  = 3,
				TYPE_CAMERA_FRAME  = 4,
			};
			enum { TYPE_COUNT = 5 };

			TYPE  type = TYPE_UNDEFINED;
			size_t  id = 0;
			size_t cid = 0;
			std::string name;
			std::string description;

			static char const *type_name[TYPE_COUNT];
		};

		virtual ~Ephemeris();

		void initialize(std::string const &path);
	
		void shutdown();

		Iterator begin() const;

		Iterator end() const;

		bool lookup(Entry &entry, std::string const &target) const;
	
		bool lookup(Entry &entry, size_t target) const;

		bool lookup(PhysicalProperties &properties, std::string const &target) const;
			 
		bool lookup(PhysicalProperties &properties, size_t target) const;
			 
		bool lookup(RenderProperties &properties, std::string const &target) const;

		bool lookup(RenderProperties &properties, size_t target) const;

		bool lookup(Elements &elements, std::string const &target, scalar_t jdn) const;
			 
		bool lookup(Elements &elements, size_t target, scalar_t jdn) const;

		static Ephemeris &instance();

	protected:
		Ephemeris();

	private:
		typedef std::vector<Elements> elements_vec_t;

		typedef std::unordered_map<std::string, size_t> id_map_t;

		typedef std::unordered_map<size_t, Entry> entry_map_t;
		typedef std::unordered_map<size_t, PhysicalProperties> physical_properties_map_t;
		typedef std::unordered_map<size_t, RenderProperties> render_properties_map_t;
		typedef std::unordered_map<size_t, elements_vec_t> elements_map_t;

		ordinal_vec_t _order;

		id_map_t _ids;

		entry_map_t _entries;	
		physical_properties_map_t _physicalProperties;
		render_properties_map_t _renderProperties;
		elements_map_t _elements;

		size_t lookup(std::string const &target) const;

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
		size_t id = lookup(target);
		if (-1 == id)
			return false;

		return lookup(entry, id);
	}

	inline bool Ephemeris::lookup(Entry &entry, size_t id) const
	{
		auto iter = _entries.find(id);
		if (iter == _entries.end())
			return false;

		entry = iter->second;

		return true;
	}

	inline bool Ephemeris::lookup(PhysicalProperties &properties, std::string const &target) const
	{
		size_t id = lookup(target);
		if (-1 == id)
			return false;

		return lookup(properties, id);
	}

	inline bool Ephemeris::lookup(RenderProperties &properties, std::string const &target) const
	{
		size_t id = lookup(target);
		if (-1 == id)
			return false;

		return lookup(properties, id);
	}

	inline bool Ephemeris::lookup(Elements &elements, std::string const &target, scalar_t jdn) const
	{
		size_t id = lookup(target);
		if (-1 == id)
			return false;

		return lookup(elements, id, jdn);
	}

	inline size_t Ephemeris::lookup(std::string const &target) const
	{
		auto iter = _ids.find(target);
		if (iter == _ids.end())
			return -1;

		return iter->second;
	}

}	///	orbit
}	///	lucid
