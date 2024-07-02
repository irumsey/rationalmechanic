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
	gal::Vector2 reticulePosition[2];
	gal::Vector2 calloutPosition[2];

	void render(gigl::Context const &context) const;

};

LUCID_ORBIT_END