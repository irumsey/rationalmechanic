#pragma once

#include <lucid/core/Constants.h>

namespace lucid {
namespace orbit {
namespace constants {

	///	 au = astronomical unit
	///	aus = plural au
	///	 ru = render unit
	/// rus = plural render unit

	LUCID_CONSTANT(seconds_per_day,                                 86400.0 );
	LUCID_CONSTANT(  meters_per_au,                          149597870700.0 );
	LUCID_CONSTANT(  aus_per_meter,   6.6845871222684454959959533702106e-12 );
	LUCID_CONSTANT(  meters_per_ru,                            1495978707.0 );
	LUCID_CONSTANT(  rus_per_meter,   6.6845871222684454959959533702106e-10 );

}	///	constants
}	///	orbit
}	///	lucid