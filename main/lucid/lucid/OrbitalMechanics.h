#pragma once

#include <unordered_map>
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

	class Frame;
	class System;

}	///	orbit
}	///	lucid

namespace lucid {

	ref class Vector3;
	ref class Context;

	///	OrbitalFrame
	///
	///	Managed wrapper for an orbital frame
	public ref class OrbitalFrame
	{
	public:
		OrbitalFrame(::lucid::orbit::Frame const *frame);

		~OrbitalFrame();

		!OrbitalFrame();

		property OrbitalFrame ^CenterFrame { OrbitalFrame ^get(); }

		property OrbitalFrame ^FirstChild { OrbitalFrame ^get(); }

		property OrbitalFrame ^NextSibling { OrbitalFrame ^get(); }

		property size_t ID { size_t get(); }

		property System::String ^Name { System::String ^get(); }

		property System::String ^Description { System::String ^get(); }

		property Vector3 ^Position { Vector3 ^get(); }

		property ::lucid::orbit::Frame const &ref { ::lucid::orbit::Frame const &get() { return *_internal; } }

		property ::lucid::orbit::Frame const *ptr { ::lucid::orbit::Frame const *get() { return  _internal; } }

	private:
		::lucid::orbit::Frame const *_internal = nullptr;

	};

	///
	///
	///
	public ref class OrbitalMechanics
	{
	public:
		typedef ::lucid::orbit::scalar_t scalar_t;

		OrbitalMechanics(System::String ^pathEphemeris);

		~OrbitalMechanics();

		!OrbitalMechanics();

		void Initialize(scalar_t dayNumber);

		void Update();

		void Render(Context ^context);

		OrbitalFrame ^RootFrame();

		OrbitalFrame ^Frame(size_t id);

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
