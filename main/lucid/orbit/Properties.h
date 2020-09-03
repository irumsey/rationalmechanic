#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Reader.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/DetailLevels.h>

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

		PhysicalProperties() = default;

		PhysicalProperties(::lucid::core::Reader &reader)
		{
			read(reader);
		}

		~PhysicalProperties() = default;

		void read(::lucid::core::Reader &reader)
		{
			reader.read(    GM);
			reader.read(  mass);
			reader.read(radius);
		}

	};

	///
	///
	///
	struct RenderProperties
	{
		DetailLevels detailLevels;

		RenderProperties() = default;

		RenderProperties(::lucid::core::Reader &reader)
		{
			read(reader);
		}

		~RenderProperties() = default;

		void read(::lucid::core::Reader &reader)
		{
			detailLevels.read(reader);
		}
	};

}	///	orbit
}	///	lucid
