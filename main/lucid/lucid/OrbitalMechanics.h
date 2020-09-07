#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Types.h>

///
///	Initial managed facade for the lucid::orbit library
///

namespace lucid {
namespace  core {

	class Clock;

}	///	orbit
}	///	lucid

namespace lucid {
namespace orbit {

	class System;

}	///	orbit
}	///	lucid

namespace lucid {

	ref class Context;

	public ref class OrbitalMechanics
	{
	public:
		typedef ::lucid::orbit::scalar_t scalar_t;

		OrbitalMechanics(System::String ^pathEphemeris);

		~OrbitalMechanics();

		!OrbitalMechanics();

		void initialize(scalar_t dayNumber);

		void update();

		void render(Context ^context);

	private:
		scalar_t const    TIME_STEP = 0.1;
		scalar_t const   TIME_LIMIT = 0.3;

		scalar_t          _wallTime = 0;
		scalar_t      _wallTimeLast = 0;
		scalar_t     _wallTimeAccum = 0;
		scalar_t           _simTime = 0;
		float32_t _frameInterpolant = 0;

		::lucid::core::Clock              *_clock = nullptr;
		::lucid::orbit::System    *_orbitalSystem = nullptr;

	};

}	///	lucid
