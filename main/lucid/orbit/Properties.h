#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Reader.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Model.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

LUCID_ORBIT_BEGIN

///	PhysicalProperties
///
///
struct PhysicalProperties
{
	scalar_t GM;
	scalar_t mass;
	scalar_t radius;

	PhysicalProperties() = default;

	PhysicalProperties(core::Reader &reader)
	{
		read(reader);
	}

	~PhysicalProperties() = default;

	void read(core::Reader &reader)
	{
		reader.read(    &GM, sizeof(scalar_t));
		reader.read(  &mass, sizeof(scalar_t));
		reader.read(&radius, sizeof(scalar_t));
	}

};

///
///
///
struct RenderProperties
{
	bool showOrbit = false;			//	the default is provided by the ephemeris
	bool bodyHighlight = false;		//	used only at runtime to toggle highlighting
	bool orbitHighlight = false;	//	used only at runtime to toggle highlighting 

	std::shared_ptr<gigl::Model> model;
	gal::Color color;
	gal::Vector4 parameters;

	RenderProperties() = default;

	RenderProperties(core::Reader &reader)
	{
		read(reader);
	}

	~RenderProperties() = default;

	void read(core::Reader &reader)
	{
		showOrbit = reader.read<bool>();

		model.reset(new gigl::Model(reader));
		reader.read(&color, sizeof(gal::Color));
		reader.read(&parameters, sizeof(gal::Vector4));
	}
};

LUCID_ORBIT_END