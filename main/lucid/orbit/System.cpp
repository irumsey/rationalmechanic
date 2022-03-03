#include "System.h"
#include "Ephemeris.h"
#include "StarCatalog.h"
#include "Frame.h"

namespace gigl = ::lucid::gigl;

namespace lucid {
namespace orbit {

	///
	///
	///

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

	///
	///
	///

	System::System(scalar_t dayNumber)
	{
		initialize(dayNumber);
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

	void System::shutdown()
	{
		_renderer.shutdown();
		_simulator.shutdown();

		delete _root;
		_root = nullptr;

		_frames.clear();
	}

	void System::attach(Frame *center, Frame *frame)
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

	void System::detach(Frame *frame)
	{
		Frame *center = frame->centerFrame;
		auto iter = _frames.find(frame->id);

		LUCID_VALIDATE(nullptr != center, "attempt to detach an already detached frame");
		LUCID_VALIDATE(_root != frame, "attempt to detach the root frame");
		LUCID_VALIDATE(frame != center, "attempt to detach a root frame");
		LUCID_VALIDATE(iter != _frames.end(), "attempt to detach a frame not managed by system");

		center->removeChild(frame);
		_frames.erase(iter);
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
		selection.tag = (Renderer::SELECT_MASK & code);

		return selection;
	}

}	///	orbit
}	///	lucid
