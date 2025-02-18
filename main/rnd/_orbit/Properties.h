#pragma once

#include <memory>
#include <lucid/core/Defines.h>
#include <lucid/gigl/Defines.h>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>

LUCID_CORE_BEGIN

class Writer;
class Reader;

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
	std::string iconPath;
	std::shared_ptr<LUCID_GIGL::Mesh> icon;	

	std::string modelPath;
	std::shared_ptr<LUCID_GIGL::Model> model;

	RenderProperties() = default;

	RenderProperties(LUCID_CORE::Reader &reader);

	~RenderProperties() = default;

	void write(LUCID_CORE::Writer &writer) const;

	void read(LUCID_CORE::Reader &reader);

};

ORBIT_END
