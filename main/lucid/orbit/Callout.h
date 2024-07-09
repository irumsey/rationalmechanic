#pragma once

#include <lucid/gal/Types.h>
#include <lucid/gigl/Defines.h>
#include <lucid/orbit/Defines.h>

LUCID_GIGL_BEGIN

class Context;

LUCID_GIGL_END

LUCID_ORBIT_BEGIN

/// 
/// 
/// 
struct Callout
{
	LUCID_GAL::Vector2 reticulePosition[2];
	LUCID_GAL::Vector2 calloutPosition[2];

	void render(LUCID_GIGL::Context const &context) const;

};

LUCID_ORBIT_END