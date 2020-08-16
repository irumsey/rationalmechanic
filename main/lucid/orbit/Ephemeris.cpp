#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
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

	inline orbit::matrix3x3_t Rx(float32_t theta)
	{
		float32_t c = ::cosf(theta);
		float32_t s = ::sinf(theta);

		return orbit::matrix3x3_t(1, 0, 0, 0, c, s, 0, -s, c);
	}

	inline orbit::matrix3x3_t Rz(float32_t theta)
	{
		float32_t c = ::cosf(theta);
		float32_t s = ::sinf(theta);

		return orbit::matrix3x3_t(c, s, 0, -s, c, 0, 0, 0, 1);
	}

}	///	anonymous

///
///
///

namespace lucid {
namespace orbit {

	Ephemeris::Ephemeris()
	{
		///	create the 'special' entry that defines the 'center' of the system

		Properties properties;
		properties.description = "solar system barycenter";

		_names.insert(std::make_pair("SSB", 0));
		_properties.insert(std::make_pair(0, properties));
		_entries.insert(std::make_pair(0, Entry()));
	}

	Ephemeris::~Ephemeris()
	{
	}

	void Ephemeris::initialize(std::string const &path)
	{
		core::FileReader reader(path);

		size_t bodyCount = 0;
		reader.read(bodyCount);
		for (size_t bodyIndex = 0; bodyIndex < bodyCount; ++bodyIndex)
		{
			Properties properties;
			Entry entry;

			size_t hid = 0;
			reader.read(hid);

			std::string target;
			reader.read(target);

			reader.read(entry.center);
			LUCID_VALIDATE(_properties.end() != _properties.find(entry.center), "'" + target + "' specifies unknown center object");

			reader.read(properties.description);
			reader.read(properties.GM);
			reader.read(properties.mass);
			reader.read(properties.radius);

			size_t elementsCount = 0;
			reader.read(elementsCount);

			for (size_t elementsIndex = 0; elementsIndex < elementsCount; ++elementsIndex)
			{
				Elements elements;
				reader.read(&elements, sizeof(elements));
				entry.elements.push_back(elements);
			}

			_names.insert(std::make_pair(target, hid));
			_properties.insert(std::make_pair(hid, properties));
			_entries.insert(std::make_pair(hid, entry));
		}
	}

	size_t Ephemeris::lookup(std::string const &target) const
	{
		auto iter = _names.find(target);
		LUCID_VALIDATE(iter != _names.end(), "unknown target '" + target + "' specified");

		return iter->second;
	}

	void Ephemeris::lookup(Properties &properties, std::string const &target) const
	{
		lookup(properties, lookup(target));
	}

	void Ephemeris::lookup(Properties &properties, size_t target) const
	{
		auto iter = _properties.find(target);
		LUCID_VALIDATE(iter != _properties.end(), "unknown target id specified");

		properties = iter->second;
	}

	size_t Ephemeris::lookup(Elements &elements, std::string const &target, float32_t jdn) const
	{
		return lookup(elements, lookup(target), jdn);
	}

	size_t Ephemeris::lookup(Elements &elements, size_t target, float32_t jdn) const
	{
		auto iter = _entries.find(target);
		LUCID_VALIDATE(iter != _entries.end(), "unknown target id specified");

		Entry const &entry = iter->second;
		size_t const count = entry.elements.size();

		LUCID_VALIDATE(0 < count, "target does not define orbital elements");

		///	find the closest entry to the given julian day.
		///	for now, it is a simple linear scan through the list.
		///	test { 

		size_t index = 0;
		float32_t a = ::fabsf(entry.elements[index].JDN - jdn);
		for (size_t i = 1; i < count; ++i)
		{
			float32_t b = ::fabsf(entry.elements[i].JDN - jdn);
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

		elements = entry.elements[index];

		return entry.center;
	}

	void Ephemeris::compute(vector3_t &position, vector3_t &velocity, std::string const &target, float32_t jdn) const
	{
		compute(position, velocity, lookup(target), jdn);
	}

	void Ephemeris::compute(vector3_t &position, vector3_t &velocity, size_t target, float32_t jdn) const
	{
		Elements elements;
		size_t cid = lookup(elements, target, jdn);

		Properties centerProperties;
		lookup(centerProperties, cid);

		float32_t const tolsq = math::constants::tol_tol<float32_t>();
		float32_t const   spd = 86400.f;
		float32_t const    dt = spd * (jdn - elements.JDN);
		float32_t const    GM = centerProperties.GM;
		float32_t const     e = elements.EC;
		float32_t const     a = elements.A;

		float32_t MA = elements.MA + dt * ::sqrtf(GM / ::powf(a, 3.f));
		MA = ::fmodf(MA, math::constants::two_pi<float32_t>());
		MA = (MA < 0.f) ? MA + math::constants::two_pi<float32_t>(): MA;

		float32_t EA[2] = { MA, 0.f };

		float32_t err = EA[0] - EA[1];
		size_t iter = 0;

		while (((err * err) > tolsq) && (iter < 5))
		{
			EA[1] = EA[0] - (EA[0] - e * ::sinf(EA[0]) - MA) / (1.f - e * ::cosf(EA[0]));
			err = EA[1] - EA[0];
			std::swap(EA[0], EA[1]);

			++iter;
		}
		
		float32_t TA = 2.f * ::atan2f(::sqrtf(1.f + e) * ::sinf(0.5f * EA[0]), ::sqrtf(1.f - e) * ::cosf(0.5f * EA[0]));
		float32_t  r = a * (1.f - e * ::cosf(EA[0]));

		position = r * vector3_t(::cosf(TA), ::sinf(TA), 0.f);
		velocity = ::sqrtf(GM * a) / r * vector3_t(-::sinf(EA[0]), ::sqrt(1.f - e * e) * ::cosf(EA[0]), 0.f);

		matrix3x3_t R = Rz(-elements.OM) * Rx(-elements.IN) * Rz(-elements.W);

		position = R * position;
		velocity = R * velocity;
	}

	Ephemeris &Ephemeris::instance()
	{
		static Ephemeris theInstance;
		return theInstance;
	}

}	///	orbit
}	///	lucid
