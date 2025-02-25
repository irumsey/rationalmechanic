#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/core/Unserializer.h>
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

	PhysicalProperties(LUCID_CORE::Unserializer &stream)
	{
		read(stream);
	}

	~PhysicalProperties() = default;

	void read(LUCID_CORE::Unserializer &stream)
	{
		stream.read(    &GM, sizeof(scalar_t));
		stream.read(  &mass, sizeof(scalar_t));
		stream.read(&radius, sizeof(scalar_t));
	}

};

///
///
///
struct RenderProperties
{
	bool showOrbit = false;						//	the default is provided by the ephemeris
	bool showIcon = false;						//	ditto above
	bool bodyHighlight = false;					//	used only at runtime to toggle highlighting
	bool orbitHighlight = false;				//	ditto above 

	std::shared_ptr<LUCID_GIGL::Model> model;

	std::shared_ptr<LUCID_GIGL::Mesh> iconMesh;	
	LUCID_GAL::Vector4 iconParameters;			//	texpos(2) and texsize(2)

	LUCID_GAL::Vector4 channel0;				//	specified in ephemeris
	LUCID_GAL::Vector4 channel1;				//	"
	LUCID_GAL::Vector4 channel2;				//	"

	RenderProperties() = default;

	RenderProperties(LUCID_CORE::Unserializer &stream)
	{
		read(stream);
	}

	~RenderProperties() = default;

	void read(LUCID_CORE::Unserializer &stream)
	{
		showOrbit = stream.read<bool>();
		showIcon = stream.read<bool>();

		// stream.nested_begin("model");
		model.reset(new LUCID_GIGL::Model(stream));
		// stream.nested_end("model");

		stream.nested_begin("icon");
		iconMesh = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(stream.read<std::string>());
		iconParameters = stream.read<LUCID_GAL::Vector4>(4);
		stream.nested_end("icon");

		channel0 = stream.read<LUCID_GAL::Vector4>(4);
		channel1 = stream.read<LUCID_GAL::Vector4>(4);
		channel2 = stream.read<LUCID_GAL::Vector4>(4);
	}
};

LUCID_ORBIT_END