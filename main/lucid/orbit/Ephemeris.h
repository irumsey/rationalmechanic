#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

#define LUCID_ORBIT_EPHEMERIS LUCID_ORBIT::Ephemeris::instance()

LUCID_ORBIT_BEGIN

struct PhysicalProperties;
struct RenderProperties;
struct OrbitalElements;
struct RotationalElements;

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

	///	the following lookup(...) methods will fail (return false)
	///	if the requested target doesn't exist or if the specified JDN
	/// doesn't fall within an exiting range of elements.  the idea
	/// is data must exist or it will fail.
	/// 
	/// important: no defaults for physical or orbital element data.

	bool lookup(Entry &entry, std::string const &target) const;
	
	bool lookup(Entry &entry, size_t target) const;

	bool lookup(PhysicalProperties &properties, std::string const &target) const;
			 
	bool lookup(PhysicalProperties &properties, size_t target) const;
			 
	bool lookup(RenderProperties &properties, std::string const &target) const;

	bool lookup(RenderProperties &properties, size_t target) const;

	bool lookup(OrbitalElements &elements, std::string const &target, scalar_t jdn) const;
			 
	bool lookup(OrbitalElements &elements, size_t target, scalar_t jdn) const;

	bool lookup(RotationalElements &elements, std::string const &target) const;

	bool lookup(RotationalElements &elements, size_t target) const;

	static Ephemeris &instance();

protected:
	Ephemeris();

private:
	typedef std::vector<OrbitalElements> elements_vec_t;

	typedef std::unordered_map<std::string, size_t> id_map_t;

	typedef std::unordered_map<size_t, Entry> entry_map_t;
	typedef std::unordered_map<size_t, PhysicalProperties> physical_properties_map_t;
	typedef std::unordered_map<size_t, RenderProperties> render_properties_map_t;
	typedef std::unordered_map<size_t, elements_vec_t> elements_map_t;
	typedef std::unordered_map<size_t, RotationalElements> rotation_map_t;

	ordinal_vec_t _order;

	id_map_t _ids;

	entry_map_t _entries;	
	physical_properties_map_t _physicalProperties;
	render_properties_map_t _renderProperties;
	elements_map_t _elements;
	rotation_map_t _rotation;

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

inline bool Ephemeris::lookup(OrbitalElements &elements, std::string const &target, scalar_t jdn) const
{
	size_t id = lookup(target);
	if (-1 == id)
		return false;

	return lookup(elements, id, jdn);
}

inline bool Ephemeris::lookup(RotationalElements &elements, std::string const &target) const
{
	size_t id = lookup(target);
	if (-1 == id)
		return false;

	return lookup(elements, id);
}

inline size_t Ephemeris::lookup(std::string const &target) const
{
	auto iter = _ids.find(target);
	if (iter == _ids.end())
		return -1;

	return iter->second;
}

LUCID_ORBIT_END