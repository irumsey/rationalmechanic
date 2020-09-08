#include "OrbitalMechanics.h"
#include "Context.h"
#include "Math.h"
#include <lucid/orbit/System.h>
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Frame.h>
#include <lucid/orbit/Utility.h>
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

	///
	///
	///

	OrbitalFrame::OrbitalFrame(::lucid::orbit::Frame const *frame)
		: _internal(frame)
	{
		LUCID_VALIDATE(nullptr != _internal, "internal consistency error");
		///	nothing to clean up...
	}

	OrbitalFrame::~OrbitalFrame()
	{
		this->!OrbitalFrame();
	}

	OrbitalFrame::!OrbitalFrame()
	{
		/// this does not own the internal frame
	}

	OrbitalFrame ^OrbitalFrame::CenterFrame::get()
	{
		return (nullptr == _internal->centerFrame) ? nullptr : gcnew OrbitalFrame(_internal->centerFrame);
	}

	OrbitalFrame ^OrbitalFrame::FirstChild::get()
	{
		return (nullptr == _internal->firstChild) ? nullptr : gcnew OrbitalFrame(_internal->firstChild);
	}

	OrbitalFrame ^OrbitalFrame::NextSibling::get()
	{
		return (nullptr == _internal->nextSibling) ? nullptr : gcnew OrbitalFrame(_internal->nextSibling);
	}

	size_t OrbitalFrame::ID::get()
	{
		return _internal->id;
	}

	System::String ^OrbitalFrame::Name::get()
	{
		return MI::marshal_as<System::String^>(_internal->name);
	}

	System::String ^OrbitalFrame::Description::get()
	{
		return MI::marshal_as<System::String^>(_internal->description);
	}

	Vector3 ^OrbitalFrame::Position::get()
	{
		return gcnew Vector3(orbit::scale(_internal->absolutePosition[1]));
	}

	///
	///
	///

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

	void OrbitalMechanics::Initialize(scalar_t dayNumber)
	{
		_orbitalSystem->initialize(dayNumber);
	}

	void OrbitalMechanics::Update()
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

	void OrbitalMechanics::Render(Context ^context)
	{
		context->Set("time", float32_t(_wallTime));
		context->Set("interpolant", _frameInterpolant);

		_orbitalSystem->render(context->ref, float32_t(_wallTime), _frameInterpolant);
	}

	OrbitalFrame ^OrbitalMechanics::RootFrame()
	{
		return gcnew OrbitalFrame(_orbitalSystem->root());
	}

	OrbitalFrame ^OrbitalMechanics::Frame(size_t id)
	{
		return gcnew OrbitalFrame(_orbitalSystem->frame(id));
	}

}	///	lucid
