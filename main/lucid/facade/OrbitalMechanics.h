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

	struct Selection;
	class      Frame;
	class     System;

}	///	orbit
}	///	lucid

namespace lucid {

	ref class Vector3;
	ref class Context;

	///
	///
	/// 
	public ref class OrbitalSelection
	{
	public:
		OrbitalSelection(orbit::Selection *selection);

		~OrbitalSelection();

		!OrbitalSelection();

		property unsigned int ID { unsigned int get(); }

		property System::String ^Description { System::String ^get();  }

		property orbit::Selection &ref { orbit::Selection &get() { return *_internal; } }

		property orbit::Selection *ptr { orbit::Selection *get() { return _internal; } }

	private:
		orbit::Selection *_internal = nullptr;

	};

	///	OrbitalFrame
	///
	///	Managed wrapper for an orbital frame
	public ref class OrbitalFrame
	{
	public:
		OrbitalFrame(orbit::Frame *frame);

		~OrbitalFrame();

		!OrbitalFrame();

		property OrbitalFrame ^CenterFrame { OrbitalFrame ^get(); }

		property OrbitalFrame ^FirstChild { OrbitalFrame ^get(); }

		property OrbitalFrame ^NextSibling { OrbitalFrame ^get(); }

		property size_t ID { size_t get(); }

		property System::String ^Name { System::String ^get(); }

		property System::String ^Description { System::String ^get(); }

		property Vector3 ^RelativePosition { Vector3 ^get(); void set(Vector3 ^value); }

		property Vector3 ^AbsolutePosition { Vector3 ^get(); }

		property orbit::Frame &ref { orbit::Frame &get() { return *_internal; } }

		property orbit::Frame *ptr { orbit::Frame *get() { return  _internal; } }

	private:
		orbit::Frame *_internal = nullptr;

	};

	///
	///
	///
	public ref class OrbitalMechanics
	{
	public:
		typedef orbit::scalar_t scalar_t;

		OrbitalMechanics(System::String ^catalog, System::String ^ephemeris, scalar_t dayNumber);

		~OrbitalMechanics();

		!OrbitalMechanics();

		void Initialize(System::String ^catalog, System::String ^ephemeris, scalar_t dayNumber);

		void Shutdown();

		OrbitalFrame ^CreateFrame(size_t type, System::String ^name, System::String ^description);

		void Attach(OrbitalFrame ^center, OrbitalFrame ^frame);

		void Detach(OrbitalFrame ^frame);

		OrbitalFrame ^RootFrame();

		OrbitalFrame ^Frame(size_t id);

		void Update();

		void Render(Context ^context);

		OrbitalSelection ^Hit(int x, int y);

		Vector3 ^InterpolatePosition(OrbitalFrame ^frame);

	private:
		scalar_t const    TIME_STEP = 0.1;
		scalar_t const   TIME_LIMIT = 0.3;

		scalar_t          _wallTime = 0;
		scalar_t      _wallTimeLast = 0;
		scalar_t     _wallTimeAccum = 0;
		scalar_t           _simTime = 0;
		float32_t _frameInterpolant = 0;

		core::Clock               *_clock = nullptr;
		orbit::System     *_orbitalSystem = nullptr;

	};

}	///	lucid
