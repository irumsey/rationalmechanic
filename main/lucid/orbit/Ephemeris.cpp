#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include "Constants.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/math/Algorithm.h>
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

			int32_t elementsCount = reader.read<int32_t>();
			rotation_vec_t pluralRotation(elementsCount);
			for (int32_t i = 0; i < elementsCount; ++i)
				reader.read(&pluralRotation[i], sizeof(RotationalElements));
			_rotation.insert(std::make_pair(target.id, pluralRotation));

			elementsCount = reader.read<int32_t>();
			elements_vec_t pluralElements(elementsCount);
			for (int32_t i = 0; i < elementsCount; ++i)
				reader.read(&pluralElements[i], sizeof(Elements));
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

bool Ephemeris::lookup(Elements &elements, size_t target, scalar_t jdn) const
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

	LUCID_VALIDATE((entries[first].JDN <= jdn) && (jdn < entries[last].JDN), "consistency error in ephemeris");

	elements = entries[first];
	return true;
}

bool Ephemeris::lookup(RotationalElements &elements, size_t target, scalar_t jdn) const
{
	auto iter = _rotation.find(target);
	if (iter == _rotation.end())
		return false;

	rotation_vec_t const &entries = iter->second;
	size_t const count = entries.size();

	/// test {
	/// for now, just return the first
	elements = entries[0];
	/// } test

	return true;
}

Ephemeris &Ephemeris::instance()
{
	static Ephemeris theInstance;
	return theInstance;
}

LUCID_ORBIT_END