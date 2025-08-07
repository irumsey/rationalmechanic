#include "Mission.h"
#include "Frame.h"
#include "Constants.h"
#include <lucid/core/Unserializer.h>
#include <lucid/core/Writer.h>
#include <lucid/core/Memory.h>

LUCID_ANONYMOUS_BEGIN

typedef orbit::Frame *(*create_func_t)(LUCID_CORE::Unserializer &stream);
extern create_func_t create_frame[orbit::Frame::TYPE_COUNT];

LUCID_ANONYMOUS_END

ORBIT_BEGIN

Mission::Mission()
{
	// test {
	read(LUCID_CORE::Unserializer("missions/theFirst.mission"));
	// } test
}

Mission::Mission(LUCID_CORE::Unserializer &stream)
{
	read(stream);
}

Mission::~Mission()
{
	shutdown();
}

void Mission::read(LUCID_CORE::Unserializer &stream)
{
	shutdown();

	_stars.initialize(LUCID_CORE::Unserializer(stream.read<std::string>()));

	int32_t frameCount = stream.read<int32_t>();
	LUCID_VALIDATE(0 < frameCount, "a mission must have at least one frame of reference");

	_root = read_frame(LUCID_CORE::Unserializer(stream.read<std::string>()));
	for (int32_t i = 1; i < frameCount; ++i)
		read_frame(LUCID_CORE::Unserializer(stream.read<std::string>()));
	_root->centerFrame = _root;

	int32_t ephemerisCount = stream.read<int32_t>();
	for (int32_t i = 0; i < ephemerisCount; ++i)
	{
		LUCID_CORE::Unserializer substream(stream.read<std::string>());
		Frame const *frame = _dictionary.lookup(substream.read<std::string>());

		int32_t elementsCount = substream.read<int32_t>();
		for (int32_t j = 0; j < elementsCount; ++j)
		{
			LUCID_VALIDATE(13 == substream.read<int32_t>(), "");
			OrbitalElements elements = substream.read<OrbitalElements>();

			// TBD: keep the file in AUs and degrees ??? {
			elements.QR = elements.QR * constants::meters_per_AU;
			elements.IN = elements.IN * constants:: rads_per_deg;
			elements.OM = elements.OM * constants:: rads_per_deg;
			elements. W = elements. W * constants:: rads_per_deg;
			elements. N = elements. N * constants:: rads_per_deg;
			elements.MA = elements.MA * constants:: rads_per_deg;
			elements.TA = elements.TA * constants:: rads_per_deg;
			elements. A = elements. A * constants::meters_per_AU;
			elements.AD = elements.AD = constants::meters_per_AU;
			// }

			_ephemeris.add(frame, elements);
		}
	}
}

Frame *Mission::read_frame(LUCID_CORE::Unserializer &stream)
{
	Frame::TYPE type = stream.read<Frame::TYPE>();
	Frame *frame = ::create_frame[type](stream);

	if (nullptr == frame)
		return nullptr;

	_dictionary.define(frame->name, frame);
	Frame *centerFrame = _dictionary[stream.read<std::string>()];

	// 'member, a detached tree has its center frame set to null.
	// and a root frame has itself as its center frame.
	if ((nullptr != centerFrame) && (frame != centerFrame))
		centerFrame->addChild(frame);
	frame->read(stream);

	return frame;
}

void Mission::shutdown()
{
	LUCID_CORE::reset_raw_ptr(_root);

	_dictionary.clear();
	_stars.clear();
	_ephemeris.clear();
}

ORBIT_END

LUCID_ANONYMOUS_BEGIN

orbit::Frame *create_null(LUCID_CORE::Unserializer &)
{
	return nullptr;
}

orbit::Frame *create_dynamic_point(LUCID_CORE::Unserializer &stream)
{
	std::string name = stream.read<std::string>();
	std::string description = stream.read<std::string>();

	return new orbit::DynamicPoint(name, description);
}

orbit::Frame *create_orbital_body(LUCID_CORE::Unserializer &stream)
{
	std::string name = stream.read<std::string>();
	std::string description = stream.read<std::string>();

	return new orbit::OrbitalBody(name, description);
}

orbit::Frame *create_dynamic_body(LUCID_CORE::Unserializer &stream)
{
	std::string name = stream.read<std::string>();
	std::string description = stream.read<std::string>();

	return new orbit::DynamicBody(name, description);
}

orbit::Frame *create_camera(LUCID_CORE::Unserializer &stream)
{
	std::string name = stream.read<std::string>();
	std::string description = stream.read<std::string>();

	return new orbit::Camera(name, description);
}

create_func_t create_frame[orbit::Frame::TYPE_COUNT] =
{
	create_null,
	create_dynamic_point,
	create_orbital_body,
	create_dynamic_body,
	create_camera,
};

LUCID_ANONYMOUS_END
