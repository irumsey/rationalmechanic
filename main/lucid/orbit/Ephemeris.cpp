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
			Entry target;

			reader.read(&target.type, sizeof(Entry::TYPE));
			reader.read(target.id);
			reader.read(target.name);
			reader.read(target.description);
			
			std::string centerName;
			reader.read(centerName);

			Entry center;
			lookup(center, centerName);

			target.cid = center.id;

			_order.push_back(target.name);
			_entries.insert(std::make_pair(target.name, target));

			///	TBD: break this, and the other types, out into their own functions...
			///	for now, orbital body is the only type with extra information
			if (Entry::TYPE_ORBITAL_BODY == target.type)
			{
				PhysicalProperties physicalProperties;

				reader.read(physicalProperties.GM);
				reader.read(physicalProperties.mass);
				reader.read(physicalProperties.radius);

				_physicalProperties.insert(std::make_pair(target.id, physicalProperties));

				RenderProperties renderProperties;

				reader.read(renderProperties.mesh);
				reader.read(&renderProperties.color, sizeof(gal::Color));
				reader.read(renderProperties.scale);
				
				_renderProperties.insert(std::make_pair(target.id, renderProperties));

				size_t elementsCount = 0;
				reader.read(elementsCount);

				elements_vec_t pluralElements(elementsCount);
				for (size_t i = 0; i < elementsCount; ++i)
					reader.read(&pluralElements[i], sizeof(Elements));
				_elements.insert(std::make_pair(target.id, pluralElements));
			}
		}
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
