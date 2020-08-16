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

		scalar_t GM = 0.f;
		scalar_t mass = 0.f;
		scalar_t radius = 0.f;
	};

}	///	orbit
}	///	lucid
