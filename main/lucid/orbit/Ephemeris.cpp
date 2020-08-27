#include "Ephemeris.h"
#include "Frame.h"
#include "Elements.h"
#include "Properties.h"
#include "Constants.h"
#include <lucid/math/Algorithm.h>
#include <lucid/math/Constants.h>
#include <lucid/core/FileReader.h>

namespace  core = ::lucid:: core;
namespace  math = ::lucid:: math;
namespace orbit = ::lucid::orbit;

///
///
///

namespace { /// anonymous

}	///	anonymous

///
///
///

namespace lucid {
namespace orbit {

	Ephemeris::Ephemeris()
	{
	}

	Ephemeris::~Ephemeris()
	{
	}

	void Ephemeris::initialize(std::string const &path)
	{
		core::FileReader reader(path);

		size_t frameCount = 0;
		reader.read(frameCount);
		for (size_t frameIndex = 0; frameIndex < frameCount; ++frameIndex)
		{
			Entry entry;

			reader.read(&entry.type, sizeof(Entry::TYPE));
			reader.read(entry.id);
			reader.read(entry.name);
			reader.read(entry.description);
			
			std::string center;
			reader.read(center);

			_order.push_back(entry.name);
			_entries.insert(std::make_pair(entry.name, entry));

			///	TBD: break this, and the other types, out into their own functions...
			///	for now, the only other type, dynamic body, is not implemented
			if (Entry::TYPE_ORBITAL_BODY == entry.type)
			{
				Properties properties;

				reader.read(properties.GM);
				reader.read(properties.mass);
				reader.read(properties.radius);

				_properties.insert(std::make_pair(entry.id, properties));

				size_t elementsCount = 0;
				reader.read(elementsCount);

				elements_vec_t blah(elementsCount);
				for (size_t i = 0; i < elementsCount; ++i)
					reader.read(&blah[i], sizeof(Elements));
				_elements.insert(std::make_pair(entry.id, blah));
			}
		}
	}

	bool Ephemeris::lookup(Properties &properties, size_t target) const
	{
		auto iter = _properties.find(target);
		if (iter == _properties.end())
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
		///	test { 

		size_t index = 0;
		scalar_t a = ::fabs(entries[index].JDN - jdn);
		for (size_t i = 1; i < count; ++i)
		{
			scalar_t b = ::fabs(entries[i].JDN - jdn);
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

		///	} test

		elements = entries[index];

		return true;
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

}	///	orbit
}	///	lucid
