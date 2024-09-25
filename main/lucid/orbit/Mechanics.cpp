#include "Mechanics.h"
#include "Ephemeris.h"
#include "StarCatalog.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/core/Clock.h>
#include <lucid/core/Error.h>
#include <sstream>
#include <iomanip>

LUCID_ORBIT_BEGIN

///
///
///

typedef Frame *(*create_func_t)(size_t id, std::string const &name, std::string const &description);

Frame *creationError(size_t, std::string const &, std::string const &)
{
	LUCID_THROW("attempt to create invalid frame type");
	return nullptr;
}

Frame *createDynamicPoint(size_t id, std::string const &name, std::string const &description)
{
	return new DynamicPoint(id, name, description);
}

Frame *createOrbitalBody(size_t id, std::string const &name, std::string const &description)
{
	OrbitalBody *body = new OrbitalBody(id, name, description);

	LUCID_VALIDATE(LUCID_ORBIT_EPHEMERIS.lookup(body->physicalProperties, id), "physical properties not found for frame: " + name);
	LUCID_VALIDATE(LUCID_ORBIT_EPHEMERIS.lookup(body->  renderProperties, id), "render properties not found for frame: " + name);
	LUCID_VALIDATE(LUCID_ORBIT_EPHEMERIS.lookup(body->rotationalElements, id), "rotational properites not found for frame: " + name);

	return body;
}

Frame *createCameraFrame(size_t id, std::string const &name, std::string const &description)
{
	CameraFrame *camera = new CameraFrame(id, name, description);

	///  TBD: anything else?

	return camera;
}

Frame *createDynamicBody(size_t id, std::string const &name, std::string const &description)
{
	return new DynamicBody(id, name, description);
}

///	ENUM LOOKUP
///	implicit use of Ephemeris::Entry::Type
create_func_t const createFrame[] =
{
	creationError,			///	TYPE_UNDEFINED
	createDynamicPoint,		///	TYPE_DYNAMIC_POINT
	createOrbitalBody,		///	TYPE_ORBITAL_BODY
	createDynamicBody,		///	TYPE_DYNAMIC_BODY
	createCameraFrame,		/// TYPE_CAMERA_FRAME
};

///
///
///

Mechanics::Mechanics(scalar_t dayNumber)
{
	initialize(dayNumber);
}

Mechanics::Mechanics()
{
	initialize(JDN::now());
}

Mechanics::~Mechanics()
{
	shutdown();
}

void Mechanics::initialize(scalar_t dayNumber)
{
	shutdown();

	_wallTime = 0;
	_wallTimeLast = 0;
	_wallTimeAccum = 0;
	_simTime = 0;
	_frameInterpolant = 0;

	_clock = LUCID_CORE::Clock::create();

	_simulator.initialize();
	_renderer.initialize("content/render.context");

	for (Ephemeris::Iterator iter = LUCID_ORBIT_EPHEMERIS.begin(); iter != LUCID_ORBIT_EPHEMERIS.end(); ++iter)
	{
		Ephemeris::Entry entry;
		LUCID_VALIDATE(LUCID_ORBIT_EPHEMERIS.lookup(entry, *iter), "ephemeris consistency error (id exists without entry)");

		Frame *frame = createFrame[entry.type](entry.id, entry.name, entry.description);
			
		LUCID_VALIDATE(_frames.end() == _frames.find(entry.id), "duplicate frame id");
		_frames.insert(std::make_pair(frame->id, frame));

		/// if the frame's id is the same as its center id, it is a root frame
		if (entry.id == entry.cid)
			continue;

		auto find = _frames.find(entry.cid);
		LUCID_VALIDATE(find != _frames.end(), "unable to find center frame for frame:" + entry.name);

		Frame *parent = find->second;
		parent->addChild(frame);
	}

	///
	///	frame 0 is the root frame
	///

	_root = _frames[0];
	_root->centerFrame = _root;

	///
	///	bootstrap simulation
	///

	_dayNumber[0] = dayNumber;
	_dayNumber[1] = dayNumber;

	update(0);
	update(0);
}

void Mechanics::shutdown()
{
	_renderer.shutdown();
	_simulator.shutdown();

	delete _root;
	_root = nullptr;

	_frames.clear();

	delete _clock;
	_clock = nullptr;
}

void Mechanics::attach(Frame *center, Frame *frame)
{
	LUCID_VALIDATE(nullptr != center, "attempt to attach to non-existent frame");
	LUCID_VALIDATE(nullptr == frame->centerFrame, "attempt to attach a non-detached frame");
	LUCID_VALIDATE(_frames.end() != _frames.find(center->id), "attempt to attach to a center not managed by system");
	LUCID_VALIDATE(_frames.end() == _frames.find(frame->id), "attempt to attach a frame already managed by system");

	_frames.insert(std::make_pair(frame->id, frame));
	center->addChild(frame);

	frame->absolutePosition[0] = center->absolutePosition[0] + frame->relativePosition[0];
	frame->absolutePosition[1] = center->absolutePosition[1] + frame->relativePosition[1];
}

void Mechanics::detach(Frame *frame)
{
	LUCID_VALIDATE(nullptr != frame, "attempt to detach invalid frame");

	Frame *center = frame->centerFrame;
	auto iter = _frames.find(frame->id);

	LUCID_VALIDATE(_frames.find(center->id) != _frames.end(), "attempt to detach a frame from a parent not managed by system");
	LUCID_VALIDATE(iter != _frames.end(), "attempt to detach a frame not managed by system");

	LUCID_VALIDATE(nullptr != center, "attempt to detach an already detached frame");
	LUCID_VALIDATE(_root != frame, "attempt to detach the root frame");
	LUCID_VALIDATE(frame != center, "attempt to detach a root frame");	/// some root but not one managed here

	center->removeChild(frame);
	_frames.erase(iter);
}

void Mechanics::update()
{
	_wallTime = _clock->time();
	scalar_t _wallTimeElapsed = _wallTime - _wallTimeLast;
	_wallTimeLast = _wallTime;

	_wallTimeElapsed = (_wallTimeElapsed > TIME_LIMIT) ? TIME_LIMIT : _wallTimeElapsed;
	_wallTimeAccum = _wallTimeAccum + _wallTimeElapsed;

	while (_wallTimeAccum >= TIME_STEP)
	{
		_simTime = _simTime + TIME_STEP;
		update(TIME_STEP);
		_wallTimeAccum = _wallTimeAccum - TIME_STEP;
	}

	_frameInterpolant = _wallTimeAccum / TIME_STEP;
}

void Mechanics::render(CameraFrame *cameraFrame, bool useFXAA)
{
	LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");
	LUCID_VALIDATE(nullptr != cameraFrame, "attempt to render using invalid camera frame");

	std::ostringstream stream;
	stream << std::fixed << std::setprecision(1) << "JDN: " << _dayNumber[1];

	_renderer.print(LUCID_GAL::Vector2(10, 10), LUCID_GAL::Vector2(32, 32), stream.str(), LUCID_GAL::Color(0, 1, 0, 1));

	// here, pass in the wall time not the sim time.  wall time is simply for rendering effects
	// which are not part of the simulation (ie particle effects, etc).  the frame interpolant
	// is for rendering the correct position of simulated entities.
	_renderer.render(_root, cameraFrame, _wallTime, _frameInterpolant, useFXAA);
}

Selection Mechanics::hit(int32_t x, int32_t y) const
{
	LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");
	return _renderer.hit(x, y);
}

scalar_t Mechanics::distance(size_t sid, size_t tid) const
{
	auto iter = _frames.find(sid);
	if (iter == _frames.end())
		return 0.0;

	Frame const *source = iter->second;

	iter = _frames.find(tid);
	if (iter == _frames.end())
		return 0.0;

	Frame const *target = iter->second;

	return LUCID_MATH::len(target->absolutePosition[1] - source->absolutePosition[1]);
}

void Mechanics::update(scalar_t delta)
{
	LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

	_dayNumber[0] = _dayNumber[1];
	_dayNumber[1] = _dayNumber[1] + delta / constants::seconds_per_day;

	/// get ephemeris time (Barycentric Dynamical Time)
	/// i keep writing out TDB because it always looks like "To Be Determined" instead.
	scalar_t Teph = LUCID_ORBIT_EPHEMERIS.time(_dayNumber[1]);
	_simulator.simulate(_root, Teph, delta);
}

LUCID_ORBIT_END