#include "Properties.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Model.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/core/Reader.h>
#include <lucid/core/Writer.h>

ORBIT_BEGIN

/// 
///
/// 

RenderProperties::RenderProperties(LUCID_CORE::Reader &reader)
{
	read(reader);
}

void RenderProperties::write(LUCID_CORE::Writer &writer) const
{
	writer.write<std::string>( iconPath);
	writer.write<std::string>(modelPath);
}

void RenderProperties::read(LUCID_CORE::Reader &reader)
{
	iconPath = reader.read<std::string>();
//	icon = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(iconPath);

	modelPath = reader.read<std::string>();
//	model = LUCID_GIGL::Resources::get<LUCID_GIGL::Model>(modelPath);
}

ORBIT_END
