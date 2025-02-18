#include "Frame.h"
#include "Algorithm.h"
#include <lucid/core/Writer.h>
#include <lucid/core/Reader.h>

ORBIT_BEGIN

///
///
///

Frame::Frame(TYPE type, std::string const &name, std::string const &description)
	: type(type)
	, name(name), description(description)
{
}

Frame::~Frame()
{
	delete firstChild;
	delete nextSibling;
}

void Frame::write(LUCID_CORE::Writer &writer) const
{
	writer.write<TYPE>(type);
	writer.write<std::string>(name);
	writer.write<std::string>(description);

	std::string centerName = (nullptr != centerFrame) ? centerFrame->name : "__null__";
	writer.write<std::string>(centerName);

	writer.write<SIM_STATE>(simState);
	writer.write<CULL_STATE>(cullState);

	writer.write(relativePosition, 2 * sizeof(vector3_t));
	writer.write(relativeVelocity, 2 * sizeof(vector3_t));

	writer.write(absolutePosition, 2 * sizeof(vector3_t));
	writer.write(absoluteVelocity, 2 * sizeof(vector3_t));

	writer.write(rotation, 2 * sizeof(quaternion_t));

	writer.write(aabbSelf, 2 * sizeof(aabb3_t));
	writer.write(aabbTotal, 2 * sizeof(aabb3_t));	
}

void Frame::read(LUCID_CORE::Reader &reader)
{
	// type				}
	// name				}
	// description		}
	// center			} read by owner to construct tree "in situ"

	simState = reader.read<SIM_STATE>();
	cullState = reader.read<CULL_STATE>();

	reader.read(relativePosition, 2 * sizeof(vector3_t));
	reader.read(relativeVelocity, 2 * sizeof(vector3_t));

	reader.read(absolutePosition, 2 * sizeof(vector3_t));
	reader.read(absoluteVelocity, 2 * sizeof(vector3_t));

	reader.read(rotation, 2 * sizeof(quaternion_t));

	reader.read(aabbSelf, 2 * sizeof(aabb3_t));
	reader.read(aabbTotal, 2 * sizeof(aabb3_t));	
}

///
///
///

DynamicPoint::DynamicPoint(std::string const &name, std::string const &description)
	: Frame(TYPE_DYNAMIC_POINT, name, description)
{
}

void DynamicPoint::apply(Algorithm *algorithm)
{
	algorithm->evaluate(this);
}

void DynamicPoint::write(LUCID_CORE::Writer &writer) const
{
	Frame::write(writer);

	renderProperties.write(writer);
}

void DynamicPoint::read(LUCID_CORE::Reader &reader)
{
	Frame::read(reader);

	renderProperties.read(reader);
}

///
///
///

OrbitalBody::OrbitalBody(std::string const &name, std::string const &description)
	: Frame(TYPE_ORBITAL_BODY, name, description)
{
}

void OrbitalBody::apply(Algorithm *algorithm)
{
	algorithm->evaluate(this);
}

void OrbitalBody::write(LUCID_CORE::Writer &writer) const
{
	Frame::write(writer);

	writer.write(&physicalProperties, sizeof(PhysicalProperties));
	renderProperties.write(writer);

	writer.write(&rotationalElements, sizeof(RotationalElements));
	writer.write(orbitalElements, 2 * sizeof(OrbitalElements));
}

void OrbitalBody::read(LUCID_CORE::Reader &reader)
{
	Frame::read(reader);

	reader.read(&physicalProperties, sizeof(PhysicalProperties));
	renderProperties.read(reader);

	reader.read(&rotationalElements, sizeof(RotationalElements));
	reader.read(orbitalElements, 2 * sizeof(OrbitalElements));	
}

///
///
///

DynamicBody::DynamicBody(std::string const &name, std::string const &description)
	: Frame(TYPE_DYNAMIC_BODY, name, description)
{
}

void DynamicBody::apply(Algorithm *algorithm)
{
	algorithm->evaluate(this);
}

void DynamicBody::write(LUCID_CORE::Writer &writer) const
{
	Frame::write(writer);

	renderProperties.write(writer);
}

void DynamicBody::read(LUCID_CORE::Reader &reader)
{
	Frame::read(reader);

	renderProperties.read(reader);
}

///
///
///
	
Camera::Camera(std::string const &name, std::string const &description)
	: Frame(TYPE_CAMERA, name, description)
{
}

void Camera::apply(Algorithm *algorithm)
{
	algorithm->evaluate(this);
}

void Camera::write(LUCID_CORE::Writer &writer) const
{
	Frame::write(writer);

	renderProperties.write(writer);

	std::string focusName = (nullptr != focus) ? focus->name : "__null__";
	writer.write(focusName);
}

void Camera::read(LUCID_CORE::Reader &reader)
{
	Frame::read(reader);

	renderProperties.read(reader);
}

void Camera::look(Frame *frame)
{
	focus = frame;
}

ORBIT_END
