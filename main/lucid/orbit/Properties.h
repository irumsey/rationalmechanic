#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Reader.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Model.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>

LUCID_ORBIT_BEGIN

///	PhysicalProperties
///
///
struct PhysicalProperties
{
	scalar_t GM = 0.f;
	scalar_t mass = 0.f;
	scalar_t radius = 0.f;

	PhysicalProperties() = default;

	PhysicalProperties(LUCID_CORE::Reader &reader)
	{
		read(reader);
	}

	~PhysicalProperties() = default;

	void read(LUCID_CORE::Reader &reader)
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
	bool showOrbit = false;						//	the default is provided by the ephemeris
	bool bodyHighlight = false;					//	used only at runtime to toggle highlighting
	bool orbitHighlight = false;				//	used only at runtime to toggle highlighting 

	std::shared_ptr<LUCID_GIGL::Model> model;

	std::shared_ptr<LUCID_GIGL::Mesh> iconMesh;	
	LUCID_GAL::Vector4 iconParameters;			//	texpos(2) and texsize(2)

	LUCID_GAL::Vector4 channel0;				//	specified in ephemeris
	LUCID_GAL::Vector4 channel1;				//	"
	LUCID_GAL::Vector4 channel2;				//	"

	RenderProperties() = default;

	RenderProperties(LUCID_CORE::Reader &reader)
	{
		read(reader);
	}

	~RenderProperties() = default;

	void read(LUCID_CORE::Reader &reader)
	{
		showOrbit = reader.read<bool>();

		model.reset(new LUCID_GIGL::Model(reader));

		iconMesh = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(reader.read<std::string>());
		reader.read(&iconParameters, sizeof(LUCID_GAL::Vector4));

		reader.read(&channel0, sizeof(LUCID_GAL::Vector4));
		reader.read(&channel1, sizeof(LUCID_GAL::Vector4));
		reader.read(&channel2, sizeof(LUCID_GAL::Vector4));
	}
};

LUCID_ORBIT_END