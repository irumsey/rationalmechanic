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

	OrbitalFrame::OrbitalFrame(::lucid::orbit::Frame *frame)
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

	Vector3 ^OrbitalFrame::RelativePosition::get()
	{
		return gcnew Vector3(orbit::scale(_internal->relativePosition[1]));
	}

	void OrbitalFrame::RelativePosition::set(Vector3 ^value)
	{
		LUCID_VALIDATE(nullptr != _internal->centerFrame, "attempt to move root frame");

		_internal->relativePosition[1] = orbit::inv_scale(value->ref);
		_internal->absolutePosition[1] = _internal->relativePosition[1] + _internal->centerFrame->absolutePosition[1];
	}

	Vector3 ^OrbitalFrame::AbsolutePosition::get()
	{
		return gcnew Vector3(orbit::scale(_internal->absolutePosition[1]));
	}

	///
	///
	///

	OrbitalMechanics::OrbitalMechanics(System::String ^ephemeris, scalar_t dayNumber)
	{
		Initialize(ephemeris, dayNumber);
	}

	OrbitalMechanics::~OrbitalMechanics()
	{
		this->!OrbitalMechanics();
	}

	OrbitalMechanics::!OrbitalMechanics()
	{
		Shutdown();
	}

	void OrbitalMechanics::Initialize(System::String ^ephemeris, scalar_t dayNumber)
	{
		Shutdown();

		theEphemeris().initialize(MI::marshal_as<std::string>(ephemeris));

		_clock = core::Clock::create();

		_orbitalSystem = new orbit::System();
		_orbitalSystem->initialize(dayNumber);

		_wallTime = 0;
		_wallTimeLast = 0;
		_wallTimeAccum = 0;
		_simTime = 0;
		_frameInterpolant = 0;
	}

	void OrbitalMechanics::Shutdown()
	{
		delete _orbitalSystem;
		_orbitalSystem = nullptr;

		delete _clock;
		_clock = nullptr;

		theEphemeris().shutdown();
	}

	OrbitalFrame ^OrbitalMechanics::CreateFrame(size_t type, System::String ^name, System::String ^description)
	{
		return gcnew OrbitalFrame(_orbitalSystem->create(type, MI::marshal_as<std::string>(name), MI::marshal_as<std::string>(description)));
	}

	void OrbitalMechanics::Attach(OrbitalFrame ^center, OrbitalFrame ^frame)
	{
		_orbitalSystem->attach(center->ptr, frame->ptr);
	}

	void OrbitalMechanics::Detach(OrbitalFrame ^frame)
	{
		_orbitalSystem->detach(frame->ptr);
	}

	void OrbitalMechanics::Update()
	{
		LUCID_PROFILE_SCOPE("OrbitalMechanics::update()");

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
		LUCID_PROFILE_SCOPE("OrbitalMechanics::render(...)");

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

	Vector3 ^OrbitalMechanics::InterpolatePosition(OrbitalFrame ^frame)
	{
		orbit::Frame const &ref = frame->ref;
		orbit::vector3_t position = math::lerp(orbit::cast(_frameInterpolant), ref.absolutePosition[0], ref.absolutePosition[1]);
		return gcnew Vector3(orbit::scale(position));
	}

}	///	lucid
