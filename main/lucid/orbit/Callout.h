#pragma once

#include <lucid/gal/Types.h>

namespace lucid {
namespace gigl {

	class Context;

}
}

namespace lucid {
namespace orbit {

	/// 
	/// 
	/// 
	struct Callout
	{
		gal::Vector2 reticulePosition[2];
		gal::Vector2 calloutPosition[2];

		void render(gigl::Context const &context) const;

	};

}	///	orbit
}	///	lucid
