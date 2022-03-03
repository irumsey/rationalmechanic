#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Simulator.h>
#include <lucid/orbit/Renderer.h>

///
///
///

namespace lucid {
namespace core{

	class Clock;

}	/// core
}	///	lucid

namespace lucid {
namespace gigl {

	class Context;

}	///	gigl
}	/// lucid

///
///
///

namespace lucid {
namespace orbit {

	class Frame;

	///	Mechanics
	///
	/// 
	class Mechanics
	{
	public:
		Mechanics(scalar_t dayNumber);

		virtual ~Mechanics();

		void initialize(scalar_t dayNumber /* add filter for frame inclusion/exclusion */);

		void shutdown();

		scalar_t currentDayNumber() const;

		Frame *root() const;

		Frame *frame(size_t id) const;

		void attach(Frame *center, Frame *frame);

		void detach(Frame *frame);

		void update();

		void render(gigl::Context &context);

		Selection hit(int32_t x, int32_t y) const;

		vector3_t interpolatedPosition(Frame const &frame) const;

	private:
		typedef std::unordered_map<size_t, Frame *> frame_map_t;

		scalar_t _dayNumber[2] = { constants::J2000<scalar_t>(), constants::J2000<scalar_t>(), };

		scalar_t const    TIME_STEP = 0.1;
		scalar_t const   TIME_LIMIT = 0.3;

		scalar_t          _wallTime = 0;
		scalar_t      _wallTimeLast = 0;
		scalar_t     _wallTimeAccum = 0;
		scalar_t           _simTime = 0;
		float32_t _frameInterpolant = 0;

		core::Clock *_clock = nullptr;

		Simulator _simulator;
		Renderer _renderer;
		
		Frame *_root = nullptr;
		frame_map_t _frames;

		void update(scalar_t delta);

		void render(::lucid::gigl::Context const &context, float32_t time, float32_t interpolant);

		LUCID_PREVENT_COPY(Mechanics);
		LUCID_PREVENT_ASSIGNMENT(Mechanics);
	};

	inline scalar_t Mechanics::currentDayNumber() const
	{
		return _dayNumber[1];
	}

	inline Frame *Mechanics::root() const
	{
		return _root;
	}

	inline Frame *Mechanics::frame(size_t id) const
	{
		auto iter = _frames.find(id);
		LUCID_VALIDATE(iter != _frames.end(), "unknown frame id specified");

		return iter->second;
	}

}	///	orbit
}	///	lucid
