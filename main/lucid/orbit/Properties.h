#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Types.h>

namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

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
		std::shared_ptr<gigl::Mesh> mesh;
		gal::Color color;
		float32_t scale = 0.f;
	};

}	///	orbit
}	///	lucid
