#include "Frame.h"
#include "Algorithm.h"

LUCID_ORBIT_BEGIN

///
///
///

size_t Frame::_instances = 0;

Frame::Frame(size_t id, std::string const &name, std::string const &description)
	: id(id)
	, name(name)
	, description(description)
{
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

void CameraFrame::look(Frame *frame)
{
	focus = frame;
}

LUCID_ORBIT_END