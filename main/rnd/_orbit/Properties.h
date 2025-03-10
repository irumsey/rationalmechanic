#pragma once

#include <memory>
#include <lucid/core/Defines.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>

LUCID_CORE_BEGIN

class Unserializer;

LUCID_CORE_END

LUCID_GIGL_BEGIN

class Mesh;
class Model;

LUCID_GIGL_END

ORBIT_BEGIN

///	PhysicalProperties
///
///
struct PhysicalProperties
{
	scalar_t GM = 0.f;		// (m^3)/(s^2)
	scalar_t mass = 0.f;	// kg
	scalar_t radius = 0.f;	// m
};

///	RenderProperties
///
///
struct RenderProperties
{
	bool showOrbit = false;
	bool showIcon = false;

	struct IconParameters
	{
		LUCID_GAL::Vector2 size;
		LUCID_GAL::Vector2 texcoord;
		LUCID_GAL::Vector2 texsize;
		LUCID_GAL::  Color color;
	};

	std::string iconPath;
	std::shared_ptr<LUCID_GIGL::Mesh> icon;	
	IconParameters iconParameters;

	struct ModelParameters
	{
		LUCID_GAL::Color diffuse;
		LUCID_GAL::Color ambient; // alpha -> specular coefficient
	};

	std::string modelPath;
	std::shared_ptr<LUCID_GIGL::Model> model;
	ModelParameters modelParameters;

	RenderProperties() = default;

	RenderProperties(LUCID_CORE::Unserializer &stream);

	~RenderProperties() = default;

	void read(LUCID_CORE::Unserializer &stream);

};

ORBIT_END
