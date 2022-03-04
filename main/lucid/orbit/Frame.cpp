#include "Frame.h"
#include "Algorithm.h"

namespace /* anonymous */ {

}	///	anonymous

namespace lucid {
namespace orbit {

	///
	///
	///

	size_t Frame::_instances = 0;

	Frame::Frame(size_t id, std::string const &name, std::string const &description)
		: id(id)
		, name(name)
		, description(description)
	{
		::memset(relativePosition, 0, 2 * sizeof(vector3_t));
		::memset(relativeVelocity, 0, 2 * sizeof(vector3_t));
		::memset(absolutePosition, 0, 2 * sizeof(vector3_t));

		++_instances;
	}

	Frame::~Frame()
	{
		delete firstChild;
		delete nextSibling;

		--_instances;
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

	///
	///
	///
	
	CameraFrame::CameraFrame(size_t id, std::string const &name, std::string const &description)
		: Frame(id, name, description)
	{
	}

	void CameraFrame::apply(Algorithm *algorithm)
	{
		algorithm->evaluate(this);
	}

	void CameraFrame::initPerspective(float fov, float aspect, float znear, float zfar)
	{
		camera.initPerspective(fov, aspect, znear, zfar);
	}

	void CameraFrame::initOrthographic(float width, float height, float znear, float zfar)
	{
		camera.initOrthographic(width, height, znear, zfar);
	}

	void CameraFrame::look(Frame *frame)
	{
		focus = frame;
	}

}	///	orbit
}	///	lucid
