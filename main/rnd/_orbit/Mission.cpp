#include "Mission.h"
#include "Frame.h"
#include <lucid/core/Reader.h>
#include <lucid/core/Writer.h>
#include <lucid/core/Memory.h>

LUCID_ANONYMOUS_BEGIN

typedef orbit::Frame *(*create_func_t)(LUCID_CORE::Reader &reader);
extern create_func_t create_frame[orbit::Frame::TYPE_COUNT];

LUCID_ANONYMOUS_END

ORBIT_BEGIN

Mission::Mission()
{
	_root = new DynamicPoint("bc1", "solar system barycenter");
	_root->centerFrame = _root;

	Frame *  sun = new OrbitalBody(  "sun",   "the sun");
	Frame *earth = new OrbitalBody("earth", "the earth");
	Frame * moon = new OrbitalBody( "moon",  "the moon");
	Frame * mars = new OrbitalBody( "mars",  "the mars");

	_root->addChild(  sun);
	  sun->addChild(earth);
	  sun->addChild( mars);
	earth->addChild( moon);

	_dictionary.define(  "sun",   sun);
	_dictionary.define("earth", earth);
	_dictionary.define( "moon",  moon);
	_dictionary.define( "mars",  mars);

	_ephemeris.add(  sun, OrbitalElements());
	_ephemeris.add(  sun, OrbitalElements());
	_ephemeris.add(earth, OrbitalElements());
	_ephemeris.add(earth, OrbitalElements());
	_ephemeris.add( moon, OrbitalElements());
	_ephemeris.add( moon, OrbitalElements());
	_ephemeris.add( mars, OrbitalElements());
	_ephemeris.add( mars, OrbitalElements());
}

Mission::Mission(LUCID_CORE::Reader &reader)
{
	read(reader);
}

Mission::~Mission()
{
	shutdown();
}

void Mission::write(LUCID_CORE::Writer &writer) const
{
	LUCID_VALIDATE(nullptr != _root, "attempt to write an empty mission");

	// serialize the tree, then write a sentinel value to indicate end-of-data.
	write(writer, _root);
	writer.write<Frame::TYPE>(Frame::TYPE_UNDEFINED);

	_ephemeris.write(_dictionary, writer);
}

void Mission::read(LUCID_CORE::Reader &reader)
{
	shutdown();

	_root = read_frame(reader);
	while (nullptr != read_frame(reader)) {}

	// even if the tree was serialized as "detached" this must take ownership.
	_root->centerFrame = _root;

	_ephemeris.read(_dictionary, reader);
}

void Mission::write(LUCID_CORE::Writer &writer, Frame const *frame) const
{
	frame->write(writer);
	for (Frame const *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		write(writer, child);
}

Frame *Mission::read_frame(LUCID_CORE::Reader &reader)
{
	Frame::TYPE type = reader.read<Frame::TYPE>();
	Frame *frame = ::create_frame[type](reader);

	if (nullptr == frame)
		return nullptr;

	_dictionary.define(frame->name, frame);
	Frame *centerFrame = _dictionary[reader.read<std::string>()];

	// 'member, a detached tree has its center frame set to null.
	// and a root frame has itself as its center frame.
	if ((nullptr != centerFrame) && (frame != centerFrame))
		centerFrame->addChild(frame);
	frame->read(reader);

	return frame;
}

void Mission::shutdown()
{
	LUCID_CORE::reset_raw_ptr(_root);

	_dictionary.clear();
	_ephemeris.clear();
}

ORBIT_END

LUCID_ANONYMOUS_BEGIN

orbit::Frame *create_null(LUCID_CORE::Reader &)
{
	return nullptr;
}

orbit::Frame *create_dynamic_point(LUCID_CORE::Reader &reader)
{
	std::string name = reader.read<std::string>();
	std::string description = reader.read<std::string>();

	return new orbit::DynamicPoint(name, description);
}

orbit::Frame *create_orbital_body(LUCID_CORE::Reader &reader)
{
	std::string name = reader.read<std::string>();
	std::string description = reader.read<std::string>();

	return new orbit::OrbitalBody(name, description);
}

orbit::Frame *create_dynamic_body(LUCID_CORE::Reader &reader)
{
	std::string name = reader.read<std::string>();
	std::string description = reader.read<std::string>();

	return new orbit::DynamicBody(name, description);
}

orbit::Frame *create_camera(LUCID_CORE::Reader &reader)
{
	std::string name = reader.read<std::string>();
	std::string description = reader.read<std::string>();

	return new orbit::Camera(name, description);
}

create_func_t create_frame[orbit::Frame::TYPE_COUNT] =
{
	create_null,
	create_dynamic_point,
	create_orbital_body,
	create_dynamic_body,
	create_camera,
};

LUCID_ANONYMOUS_END
