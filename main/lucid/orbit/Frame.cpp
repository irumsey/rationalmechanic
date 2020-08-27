#include "Frame.h"
#include "Algorithm.h"

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

	Frame::~Frame()
	{
		delete firstChild;
		delete nextSibling;
	}

	///
	///
	///

	DynamicPoint::DynamicPoint(size_t id, std::string const &name, std::string const &description)
		: Frame(id, name, description)
	{
	}

	DynamicPoint::~DynamicPoint()
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
	}

	OrbitalBody::~OrbitalBody()
	{
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

	DynamicBody::~DynamicBody()
	{
	}

	void DynamicBody::apply(Algorithm *algorithm)
	{
		algorithm->evaluate(this);
	}


}	///	orbit
}	///	lucid
