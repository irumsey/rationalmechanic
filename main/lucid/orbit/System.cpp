#include "System.h"
#include "Ephemeris.h"
#include "StarCatalog.h"
#include "Frame.h"
#include <sstream>

namespace gigl = ::lucid::gigl;

namespace lucid {
namespace orbit {

namespace /* anonymous */ {

	inline Ephemeris &theEphemeris()
	{
		return Ephemeris::instance();
	}

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
		return new OrbitalBody(id, name, description);
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
	};

}	///	anonymous


	///
	///
	///

	System::System()
	{
	}

	System::~System()
	{
		shutdown();
	}

	void System::initialize(scalar_t dayNumber)
	{
		shutdown();

		_simulator.initialize();
		_renderer.initialize();

		for (Ephemeris::Iterator iter = theEphemeris().begin(); iter != theEphemeris().end(); ++iter)
		{
			Ephemeris::Entry entry;
			LUCID_VALIDATE(theEphemeris().lookup(entry, *iter), "ephemeris consistency error (id exists without entry)");

			Frame *frame = createFrame[entry.type](entry.id, entry.name, entry.description);
			
			LUCID_VALIDATE(_frames.end() == _frames.find(entry.id), "duplicate frame id");
			_frames.insert(std::make_pair(frame->id, frame));

			if (entry.id == entry.cid)
				continue;

			auto find = _frames.find(entry.cid);
			LUCID_VALIDATE(find != _frames.end(), "unable to find center frame for frame:" + entry.name);

			Frame *parent = find->second;
			parent->addChild(frame);
		}

		_root = _frames[0];

		///
		///	bootstrap simulation
		///

		_dayNumber[0] = dayNumber;
		_dayNumber[1] = dayNumber;

		update(0);
		update(0);
	}

	void System::shutdown()
	{
		_renderer.shutdown();
		_simulator.shutdown();

		for each (auto iter in _frames)
			delete iter.second;
		_frames.clear();

		_root = nullptr;
	}

	Frame *System::create(size_t type, std::string const &name, std::string const &description)
	{
		///	TBD: find better way (jpl horizons -> local file -> ephemeris -> frame type, plus c++ -> c#)
		LUCID_VALIDATE((0 < type) && (type < 4), "frame type id out of range");

		Frame *frame = createFrame[type](genFrameID(), name, description);
		LUCID_VALIDATE(_frames.end() == _frames.find(frame->id), "internal error: new frame id clashes with an existing");

		_frames.insert(std::make_pair(frame->id, frame));
	
		return frame;
	}

	void System::attach(Frame *center, Frame *frame)
	{
		LUCID_VALIDATE(_frames.end() != _frames.find(center->id), "attempt to attach to center not managed by system");
		LUCID_VALIDATE(_frames.end() != _frames.find(frame ->id), "attempt to attach frame not managed by system");

		center->addChild(frame);	
	}

	void System::detach(Frame *frame)
	{
		LUCID_VALIDATE(_frames.end() != _frames.find(frame->id), "attempt to detach frame not managed by system");

		Frame *center = frame->centerFrame;
		LUCID_VALIDATE(nullptr != center, "attempt to detach root frame");

		center->removeChild(frame);
	}

	void System::update(scalar_t delta)
	{
		LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

		_dayNumber[0] = _dayNumber[1];
		_dayNumber[1] = _dayNumber[1] + delta;

		_simulator.simulate(_root, _dayNumber[1], delta);
	}

	void System::render(gigl::Context const &context, float32_t time, float32_t interpolant)
	{
		LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

		_renderer.render(_root, context, time, interpolant);
	}

	Selection System::hit(int32_t x, int32_t y) const
	{
		LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

		Selection selection;
		uint32_t code = _renderer.hit(x, y);
		if (0 == code)
			return selection;

		selection.type = (Selection::TYPE)((0xf0000000 & code) >> Renderer::SELECT_SHIFT);
		selection.id = (Renderer::SELECT_MASK & code);

		// test {
		std::stringstream ss;
		if (Selection::TYPE_STAR == selection.type)
		{
			StarCatalog const &catalog = StarCatalog::instance();
			StarCatalog::Entry const &entry = catalog[selection.id];

			ss << "Star (xno): " << entry.xno << " type: " << entry.type << " mag: " << entry.magnitude;
			selection.description = ss.str();
		}
		if (Selection::TYPE_FRAME == selection.type)
		{
			Ephemeris const &ephemeris = Ephemeris::instance();
			Ephemeris::Entry entry;
			ephemeris.lookup(entry, selection.id);
			
			ss << Ephemeris::Entry::type_name[entry.type] << ": " << entry.name;
			selection.description = ss.str();
		}
		if (Selection::TYPE_ORBIT == selection.type)
		{
			Ephemeris const &ephemeris = Ephemeris::instance();
			Ephemeris::Entry entry;
			ephemeris.lookup(entry, selection.id);

			ss << "Orbit: " << entry.name;
			selection.description = ss.str();
		}
		// } test

		return selection;
	}

}	///	orbit
}	///	lucid
