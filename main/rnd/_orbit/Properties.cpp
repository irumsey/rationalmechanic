#include "Properties.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Model.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/core/Unserializer.h>

ORBIT_BEGIN

/// 
///
/// 

RenderProperties::RenderProperties(LUCID_CORE::Unserializer &stream)
{
	read(stream);
}

void RenderProperties::read(LUCID_CORE::Unserializer &stream)
{
	showOrbit = stream.read<bool>();
	showIcon = stream.read<bool>();

	stream.nested_begin();
	iconPath = stream.read<std::string>();
//	icon = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(iconPath);
	stream.read(&iconParameters, stream.read<int32_t>());
	stream.nested_end();

	stream.nested_begin();
	modelPath = stream.read<std::string>();
//	model = LUCID_GIGL::Resources::get<LUCID_GIGL::Model>(modelPath);
	stream.read(&modelParameters, stream.read<int32_t>());
	stream.nested_end();
}

ORBIT_END
