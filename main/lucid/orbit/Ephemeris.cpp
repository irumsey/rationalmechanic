#include "Ephemeris.h"
#include "Utility.h"
#include "Elements.h"
#include "Properties.h"
#include "Constants.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/math/Math.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

LUCID_ORBIT_BEGIN

//
//
//

char const *Ephemeris::Entry::type_name[TYPE_COUNT] = {
	"Undefined",
	"Dynamic Point",
	"Orbital Body",
	"Dynamic Body",
	"Camera Frame",
};

//
//
//

Ephemeris::Ephemeris()
{
}

Ephemeris::~Ephemeris()
{
	shutdown();
}

void Ephemeris::initialize(std::string const &path)
{
	shutdown();

	LUCID_CORE::Reader &reader = LUCID_CORE::FileReader(path);

	///
	///	load the data required to compute Barycentric Dynamical Time
	/// AKA ephemeris time
	/// 

	_tai_tt = reader.read<scalar_t>();

	_meanAnomaly[0] = reader.read<scalar_t>();
	_meanAnomaly[1] = reader.read<scalar_t>();

	_eccentricAnomaly[0] = reader.read<scalar_t>();
	_eccentricAnomaly[1] = reader.read<scalar_t>();

	int32_t count = reader.read<int32_t>();
	_leapSeconds.resize(count);

	for (int32_t i = 0; i < count; ++i)
	{
		Date date;
		date.year = reader.read<uint32_t>();
		date.month = reader.read<uint32_t>();
		date.day = reader.read<uint32_t>();

		_leapSeconds[i].first = JDN::from(date, Time());
		_leapSeconds[i].second = reader.read<scalar_t>();
	}

	///
	///	load the frames and their orbital elements
	/// 

	int32_t frameCount = reader.read<int32_t>();
	for (int32_t frameIndex = 0; frameIndex < frameCount; ++frameIndex)
	{
		Entry target;

		reader.read(&target.type, sizeof(Entry::TYPE));
			
		target.id = reader.read<int32_t>();
		target.name = reader.read<std::string>();
		target.description = reader.read<std::string>();
			
		std::string centerName = reader.read<std::string>();

		Entry center;
		lookup(center, centerName);

		target.cid = center.id;

		_order.push_back(target.name);

		_ids.insert(std::make_pair(target.name, target.id));
		_entries.insert(std::make_pair(target.id, target));

		///	TBD: break this, and the other types, out into their own functions...
		///	for now, orbital body is the only type with extra information
		if (Entry::TYPE_ORBITAL_BODY == target.type)
		{
			_physicalProperties.insert(std::make_pair(target.id, PhysicalProperties(reader)));
			_renderProperties.insert(std::make_pair(target.id, RenderProperties(reader)));

			RotationalElements rotationalElements;
			reader.read(&rotationalElements, sizeof(RotationalElements));
			_rotation.insert(std::make_pair(target.id, rotationalElements));

			size_t elementsCount = reader.read<int32_t>();
			elements_vec_t pluralElements(elementsCount);
			for (int32_t i = 0; i < elementsCount; ++i)
				reader.read(&pluralElements[i], sizeof(OrbitalElements));
			_elements.insert(std::make_pair(target.id, pluralElements));
		}
	}

	LUCID_CORE::log("INFO", "Ephemeris initialized using: " + path);
}

void Ephemeris::shutdown()
{
	_order.clear();
	_entries.clear();
	_physicalProperties.clear();
	_renderProperties.clear();
	_rotation.clear();
	_elements.clear();

	_leapSeconds.clear();

	_alreadyWarned = false;
}

scalar_t Ephemeris::time(scalar_t jdn) const
{
	size_t count = _leapSeconds.size();

	if (count < 2)
	{
		warnOnce("insufficient number of entries in leap second list");
		return jdn;
	}

	size_t first = 0;
	size_t last = count - 1;
	size_t mid = (last + first) >> 1;

	if (jdn < _leapSeconds[first].first)
	{
		warnOnce("specified JDN comes before first leap second entry");
		return jdn;
	}

	if (_leapSeconds[last].second < jdn)
	{
		warnOnce("specified JDN comes after last leap second entry");
		return time(jdn, _leapSeconds[last].second);
	}

	while ((first != mid) && (mid != last))
	{
		if ((_leapSeconds[first].first <= jdn) && (jdn < _leapSeconds[mid].first))
			last = mid;
		else
			first = mid;
		mid = (last + first) >> 1;
	}
	
	LUCID_VALIDATE(
		(_leapSeconds[first].first <= jdn) && (jdn < _leapSeconds[last].first),
		"consistency error in ephemeris leap seconds list"
	);

	return time(jdn, _leapSeconds[first].second);
}

bool Ephemeris::lookup(PhysicalProperties &properties, size_t target) const
{
	auto iter = _physicalProperties.find(target);
	if (iter == _physicalProperties.end())
		return false;

	properties = iter->second;

	return true;
}

bool Ephemeris::lookup(RenderProperties &properties, size_t target) const
{
	auto iter = _renderProperties.find(target);
	if (iter == _renderProperties.end())
		return false;

	properties = iter->second;

	return true;
}

bool Ephemeris::lookup(OrbitalElements &elements, size_t target, scalar_t jdn) const
{
	auto iter = _elements.find(target);
	if (iter == _elements.end())
	{
		LUCID_CORE::log("ERR", "Ephemeris: specified target not found");
		return false;
	}

	elements_vec_t const &entries = iter->second;
	size_t const count = entries.size();

	if (count < 2)
	{
		LUCID_CORE::log("WARN", "Ephemeris: insufficient number of entries");
		return false;
	}

	size_t first = 0;
	size_t last = count - 1;
	size_t mid = (last + first) >> 1;

	if(!((entries[first].JDN <= jdn) && (jdn < entries[last].JDN)))
	{
		LUCID_CORE::log("WARN", "Ephemeris: specified JDN out of bounds");
		return false;
	}

	while ((first != mid) && (mid != last))
	{
		if ((entries[first].JDN <= jdn) && (jdn < entries[mid].JDN))
			last = mid;
		else
			first = mid;
		mid = (last + first) >> 1;
	}
	LUCID_VALIDATE((entries[first].JDN <= jdn) && (jdn < entries[last].JDN), "consistency error in ephemeris orbital elements");

	elements = entries[first];
	return true;
}

bool Ephemeris::lookup(RotationalElements &elements, size_t target) const
{
	auto iter = _rotation.find(target);
	if (iter == _rotation.end())
		return false;

	elements = iter->second;

	return true;
}

Ephemeris &Ephemeris::instance()
{
	static Ephemeris theInstance;
	return theInstance;
}

void Ephemeris::warnOnce(std::string const &msg) const
{
	if (_alreadyWarned)
		return;
	_alreadyWarned = true;

	LUCID_CORE::log("WARN", msg);
}

LUCID_ORBIT_END