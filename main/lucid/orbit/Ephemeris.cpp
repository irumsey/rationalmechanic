#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include "Constants.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>

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
		return false;

	elements_vec_t const &entries = iter->second;
	size_t const count = entries.size();

	if (0 == count)
		return false;

	///	find the closest entry to the given day number.
	///	for now, it is a simple linear scan through the list which
	///	exits when the difference would increase.

	size_t index = 0;
	scalar_t a = LUCID_MATH::abs(entries[index].JDN - jdn);
	for (size_t i = 1; i < count; ++i)
	{
		scalar_t b = LUCID_MATH::abs(entries[i].JDN - jdn);
		if (b < a)
		{
			index = i;
			a = b;
		}
		else
		{
			break;
		}
	}

	elements = entries[index];

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