#pragma once

#include <string>
#include <lucid/orbit/Types.h>

namespace lucid {
namespace orbit {

	///	Properties
	///
	///	Physical properties
	struct Properties
	{
		std::string description;

		float32_t GM = 0.f;
		float32_t mass = 0.f;
		float32_t radius = 0.f;
	};

}	///	orbit
}	///	lucid
