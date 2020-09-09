#include "Frame.h"
#include "Ephemeris.h"
#include "Algorithm.h"
#include <lucid/core/Error.h>

namespace orbit = ::lucid::orbit;

namespace /* anonymous */ {

	inline orbit::Ephemeris &theEphemeris()
	{
		return orbit::Ephemeris::instance();
	}

}	///	anonymous

namespace lucid {
namespace orbit {

	///
	///
	///

	Frame::Frame(size_t id, std::string const &name, std::string const &description)
		: id(id)
		, name(name)
		, description(description)
	{
		::memset(relativePosition, 0, 2 * sizeof(vector3_t));
		::memset(relativeVelocity, 0, 2 * sizeof(vector3_t));
		::memset(absolutePosition, 0, 2 * sizeof(vector3_t));
	}

	///
	///
	///

	DynamicPoint::DynamicPoint(size_t id, std::string const &name, std::string const &description)
		: Frame(id, name, description)
	{
	}

	void DynamicPoint::apply(Algorithm *algorithm)
	{
		algorithm->evaluate(this);
	}

	///
	///
	///

	OrbitalBody::OrbitalBody(size_t id, std::string const &name, std::string const &description)
		: Frame(id, name, description)
	{
		LUCID_VALIDATE(theEphemeris().lookup(physicalProperties, id), "consistency error: properties not found for frame");
		LUCID_VALIDATE(theEphemeris().lookup(  renderProperties, id), "consistency error: properties not found for frame");
	}

	void OrbitalBody::apply(Algorithm *algorithm)
	{
		algorithm->evaluate(this);
	}

	///
	///
	///

	DynamicBody::DynamicBody(size_t id, std::string const &name, std::string const &description)
		: Frame(id, name, description)
	{
	}

	void DynamicBody::apply(Algorithm *algorithm)
	{
		algorithm->evaluate(this);
	}


}	///	orbit
}	///	lucid
