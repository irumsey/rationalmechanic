#pragma once

#include <lucid/orbit/Types.h>

namespace lucid {
namespace core {

	class Clock;

}	///	orbit
}	///	lucid

namespace lucid {
namespace orbit {

	class System;

}	///	orbit
}	///	lucid

namespace Lucid {
namespace Math {

	ref class Vector3;

}	///	Math
}	///	Lucid

namespace Lucid {
namespace GIGL {

	ref class Context;

}	///	GIGL
}	///	Lucid

namespace Lucid {
namespace Orbit {

	ref class Frame;
	ref class Selection;

	///	Mechanics
	///
	///	Note: currently breaking the design choice of the
	/// managed library only wrapping non-managed.  this
	/// is adding functionality in the form of time-keeping
	/// which should either be pushed up or down eventually.
	public ref class Mechanics
	{
	public:
		Mechanics(double dayNumber);

		~Mechanics();

		!Mechanics();

		void Initialize(double dayNumber);

		void Shutdown();

		property Frame ^Root { Frame ^get(); };

		property Frame ^default[size_t] { Frame ^get(size_t id); };

		void Attach(Frame ^center, Frame ^frame);

		void Detach(Frame ^frame);

		void Update();

		void Render(GIGL::Context ^context);

		Selection ^Hit(int x, int y);

		// TBD: example of what should not be here (but is since functionality was added here) {
		Math::Vector3 ^InterpolatePosition(Frame ^frame);
		// } TBD

	private:
		typedef ::lucid::orbit::scalar_t scalar_t;

		scalar_t const    TIME_STEP = 0.1;
		scalar_t const   TIME_LIMIT = 0.3;

		scalar_t          _wallTime = 0;
		scalar_t      _wallTimeLast = 0;
		scalar_t     _wallTimeAccum = 0;
		scalar_t           _simTime = 0;
		float32_t _frameInterpolant = 0;

		::lucid::core::Clock *_clock = nullptr;
		::lucid::orbit::System *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
