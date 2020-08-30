#pragma once

#include <lucid/core/Constants.h>

namespace lucid {
namespace orbit {
namespace constants {

	///	 AU = astronomical unit
	///	 RU = render unit

	LUCID_CONSTANT(          J2000,                                2451544.0);
	LUCID_CONSTANT(seconds_per_day,                                 86400.0 );
	LUCID_CONSTANT(  meters_per_AU,                          149597870700.0 );
	LUCID_CONSTANT(  AUs_per_meter,   6.6845871222684454959959533702106e-12 );
	LUCID_CONSTANT(  meters_per_RU,                            1495978707.0 );
	LUCID_CONSTANT(  RUs_per_meter,   6.6845871222684454959959533702106e-10 );

}	///	constants
}	///	orbit
}	///	lucid