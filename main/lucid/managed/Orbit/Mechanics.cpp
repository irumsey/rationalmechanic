#include "Mechanics.h"
#include "Frame.h"
#include "Selection.h"
#include <lucid/orbit/System.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Utility.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Clock.h>

#include <lucid/managed/GIGL/Context.h>
#include <lucid/managed/Math/Types.h>

namespace /* anonymous */ {

	namespace  core = ::lucid:: core;
	namespace  math = ::lucid:: math;
	namespace orbit = ::lucid::orbit;

}	///	anonymous

namespace Lucid {
namespace Orbit {

	Mechanics::Mechanics(double dayNumber)
	{
		Initialize(dayNumber);
	}

	Mechanics::~Mechanics()
	{
		this->!Mechanics();
	}

	Mechanics::!Mechanics()
	{
		Shutdown();
	}

	void Mechanics::Initialize(double dayNumber)
	{
		Shutdown();

		try
		{
			_clock = core::Clock::create();
			_internal = new orbit::System(dayNumber);
		}
		catch (core::Error const &error)
		{
			int nop = 0;
		}

		_wallTime = 0;
		_wallTimeLast = 0;
		_wallTimeAccum = 0;
		_simTime = 0;
		_frameInterpolant = 0;
	}

	void Mechanics::Shutdown()
	{
		delete _internal;
		_internal = nullptr;

		delete _clock;
		_clock = nullptr;
	}

	Frame ^Mechanics::Root::get()
	{
		return Frame::Wrap(_internal->root());
	}

	Frame ^Mechanics::default::get(size_t id)
	{
		return Frame::Wrap(_internal->frame(id));
	}

	void Mechanics::Attach(Frame ^center, Frame ^frame)
	{
		_internal->attach(center->ptr, frame->ptr);
	}

	void Mechanics::Detach(Frame ^frame)
	{
		_internal->detach(frame->ptr);
	}

	void Mechanics::Update()
	{
		LUCID_PROFILE_SCOPE("Mechanics::Update()");

		_wallTime = _clock->time();
		scalar_t _wallTimeElapsed = _wallTime - _wallTimeLast;
		_wallTimeLast = _wallTime;

		_wallTimeElapsed = (_wallTimeElapsed > TIME_LIMIT) ? TIME_LIMIT : _wallTimeElapsed;
		_wallTimeAccum += _wallTimeElapsed;

		while (_wallTimeAccum >= TIME_STEP)
		{
			_simTime += TIME_STEP;
			_internal->update(TIME_STEP);
			_wallTimeAccum -= TIME_STEP;
		}

		_frameInterpolant = (float32_t)(_wallTimeAccum / TIME_STEP);
	}

	void Mechanics::Render(GIGL::Context ^context)
	{
		LUCID_PROFILE_SCOPE("OrbitalMechanics::Render(...)");

		context->Set("time", float32_t(_wallTime));
		context->Set("interpolant", _frameInterpolant);

		_internal->render(context->ref, float32_t(_wallTime), _frameInterpolant);
	}

	Selection ^Mechanics::Hit(int x, int y)
	{
		return gcnew Selection(_internal->hit(x, y));
	}

	Math::Vector3 ^Mechanics::InterpolatePosition(Frame ^frame)
	{
		orbit::Frame const &ref = frame->ref;
		::lucid::orbit::vector3_t position = math::lerp(orbit::cast(_frameInterpolant), ref.absolutePosition[0], ref.absolutePosition[1]);
		return gcnew Math::Vector3(orbit::scale(position));
	}

}	///	Orbit
}	///	Lucid
