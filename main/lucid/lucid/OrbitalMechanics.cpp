#include "OrbitalMechanics.h"
#include "Context.h"
#include <lucid/orbit/System.h>
#include <lucid/orbit/Ephemeris.h>
#include <lucid/core/Clock.h>
#include <lucid/core/Profiler.h>
#include <msclr/marshal_cppstd.h>

namespace    MI = msclr::interop;
namespace  core = ::lucid::core;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */ {

	inline orbit::Ephemeris &theEphemeris()
	{
		return orbit::Ephemeris::instance();
	}

}	///	anonymous

namespace lucid {

	OrbitalMechanics::OrbitalMechanics(System::String ^pathEphemeris)
	{
		theEphemeris().initialize(MI::marshal_as<std::string>(pathEphemeris));

		_clock = core::Clock::create();
		_orbitalSystem = new orbit::System();
	}

	OrbitalMechanics::~OrbitalMechanics()
	{
		this->!OrbitalMechanics();
	}

	OrbitalMechanics::!OrbitalMechanics()
	{
		delete _orbitalSystem;
		delete _clock;
	}

	void OrbitalMechanics::initialize(scalar_t dayNumber)
	{
		_orbitalSystem->initialize(dayNumber);
	}

	void OrbitalMechanics::update()
	{
		_wallTime = _clock->time();
		scalar_t _wallTimeElapsed = _wallTime - _wallTimeLast;
		_wallTimeLast = _wallTime;

		_wallTimeElapsed = (_wallTimeElapsed > TIME_LIMIT) ? TIME_LIMIT : _wallTimeElapsed;
		_wallTimeAccum += _wallTimeElapsed;

		while (_wallTimeAccum >= TIME_STEP)
		{
			_simTime += TIME_STEP;
			_orbitalSystem->update(TIME_STEP);
			_wallTimeAccum -= TIME_STEP;
		}

		_frameInterpolant = (float32_t)(_wallTimeAccum / TIME_STEP);
	}

	void OrbitalMechanics::render(Context ^context)
	{
		context->Set("time", float32_t(_wallTime));
		context->Set("interpolant", _frameInterpolant);

		_orbitalSystem->render(context->ref, float32_t(_wallTime), _frameInterpolant);
	}

}	///	lucid
