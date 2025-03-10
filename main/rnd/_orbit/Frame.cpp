#include "Frame.h"
#include "Algorithm.h"
#include <lucid/core/Unserializer.h>

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

void Frame::read(LUCID_CORE::Unserializer &stream)
{
	// type					}
	// name					}
	// description			}
	// center				} read by owner to construct tree "in place"

	stream.nested_begin();
	renderProperties.read(stream);
	stream.nested_end();

	// simState				}
	// cullState			}
	// relativePosition		}
	// relativeVelocity		}
	// absolutePosition		}
	// absoluteVelocity		}
	// rotation				}
	// aabbSelf				}
	// aabbTotal			} to promote data reuse, simulation data is read separately
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

void DynamicPoint::read(LUCID_CORE::Unserializer &stream)
{
	Frame::read(stream);

	// TBD: any specific data
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

void OrbitalBody::read(LUCID_CORE::Unserializer &stream)
{
	Frame::read(stream);

	scalar_t v = 1.3271244004193939e+20;

	stream.read(&physicalProperties, stream.read<int32_t>());
	stream.read(&rotationalElements, stream.read<int32_t>());

	// orbitalElements		} simulation data read separately
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

void DynamicBody::read(LUCID_CORE::Unserializer &stream)
{
	Frame::read(stream);

	// TBD: any specific data
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

void Camera::read(LUCID_CORE::Unserializer &stream)
{
	Frame::read(stream);

	// TBD: any specific data
}

void Camera::look(Frame *frame)
{
	focus = frame;
}

ORBIT_END
