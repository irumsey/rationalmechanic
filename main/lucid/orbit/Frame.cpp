#include "Frame.h"
#include "Algorithm.h"

LUCID_ORBIT_BEGIN

///
///
///

Frame::Frame(int32_t id, std::string const &name, std::string const &description)
	: id(id)
	, name(name)
	, description(description)
{
}

Frame::~Frame()
{
	delete firstChild;
	delete nextSibling;
}

///
///
///

DynamicPoint::DynamicPoint(int32_t id, std::string const &name, std::string const &description)
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

OrbitalBody::OrbitalBody(int32_t id, std::string const &name, std::string const &description)
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

DynamicBody::DynamicBody(int32_t id, std::string const &name, std::string const &description)
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
	
CameraFrame::CameraFrame(int32_t id, std::string const &name, std::string const &description)
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