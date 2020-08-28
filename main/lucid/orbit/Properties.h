#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Types.h>

namespace lucid {
namespace orbit {

	///	PhysicalProperties
	///
	///
	struct PhysicalProperties
	{
		scalar_t GM = 0.0;
		scalar_t mass = 0.0;
		scalar_t radius = 0.0;
	};

	///	RenderProperties
	///
	///
	struct RenderProperties
	{
		gal::Color color;
		float32_t emit = 0.f;
		float32_t scale = 0.f;
	};

}	///	orbit
}	///	lucid
