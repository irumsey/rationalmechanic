#include "System.h"
#include "Ephemeris.h"
#include "Frame.h"
#include <unordered_map>

namespace gigl = ::lucid::gigl;

namespace lucid {
namespace orbit {

namespace /* anonymous */ {

	inline Ephemeris &theEphemeris()
	{
		return Ephemeris::instance();
	}

	typedef Frame *(*create_func_t)(Ephemeris::Entry const &entry);

	Frame *creationError(Ephemeris::Entry const &)
	{
		LUCID_THROW("attempt to create invalid frame type");
		return nullptr;
	}

	Frame *createDynamicPoint(Ephemeris::Entry const &entry)
	{
		return new DynamicPoint(entry.id, entry.name, entry.description);
	}

	Frame *createOrbitalBody(Ephemeris::Entry const &entry)
	{
		return new OrbitalBody(entry.id, entry.name, entry.description);
	}

	Frame *createDynamicBody(Ephemeris::Entry const &entry)
	{
		return new DynamicBody(entry.id, entry.name, entry.description);
	}

	create_func_t const createFrame[] =
	{
		creationError,
		createDynamicPoint,
		createOrbitalBody,
		createDynamicBody,
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

		std::unordered_map<size_t, Frame *> frames;
		for (Ephemeris::Iterator iter = theEphemeris().begin(); iter != theEphemeris().end(); ++iter)
		{
			Ephemeris::Entry entry;
			LUCID_VALIDATE(theEphemeris().lookup(entry, *iter), "ephemeris consistency error (id exists without entry)");

			Frame *frame = createFrame[entry.type](entry);
			
			LUCID_VALIDATE(frames.end() == frames.find(entry.id), "duplicate frame id");
			frames.insert(std::make_pair(frame->id, frame));

			if (entry.id == entry.cid)
				continue;

			auto find = frames.find(entry.cid);
			LUCID_VALIDATE(find != frames.end(), "unable to find center frame for frame:" + entry.name);

			Frame *parent = find->second;
			parent->addChild(frame);
		}

		_root = frames[0];

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
	}

	void System::update(scalar_t delta)
	{
		LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

		_dayNumber[0] = _dayNumber[0] + delta;
		std::swap(_dayNumber[0], _dayNumber[1]);

		_simulator.simulate(_root, _dayNumber[1], delta);
	}

	void System::render(gigl::Context const &context, float32_t time, float32_t interpolant)
	{
		LUCID_VALIDATE(nullptr != _root, "attempt to use uninitialized system");

		_renderer.render(_root, context, time, interpolant);
	}

}	///	orbit
}	///	lucid
