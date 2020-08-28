#include "Simulator.h"
#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include "Frame.h"
#include "Utility.h"

namespace { /// anonymous

	inline ::lucid::orbit::Ephemeris &theEphemeris()
	{
		return ::lucid::orbit::Ephemeris::instance();
	}

}	/// anonymous

namespace lucid {
namespace orbit {

	Simulator::Simulator()
	{
	}

	Simulator::~Simulator()
	{
		shutdown();
	}

	void Simulator::evaluate(DynamicPoint *point)
	{
		LUCID_PROFILE_BEGIN("simulating dynamic points");

		///	TBD: implement

		LUCID_PROFILE_END();
	}

	void Simulator::evaluate(OrbitalBody *body)
	{
		LUCID_PROFILE_BEGIN("simulating orbital bodies");

		Frame const *center = body->centerFrame;
		if (nullptr == center)
			return;

		theEphemeris().lookup(body->elements[0], body->id, _dayNumber);

		PhysicalProperties centerProperties;
		theEphemeris().lookup(centerProperties, center->id);

		kinematicsFromElements(body->relativePosition[0], body->relativeVelocity[0], centerProperties, body->elements[0], _dayNumber);
		body->absolutePosition[0] = body->relativePosition[0] + center->absolutePosition[1];

		std::swap(body->        elements[0], body->        elements[1]);
		std::swap(body->relativePosition[0], body->relativePosition[1]);
		std::swap(body->absolutePosition[0], body->absolutePosition[1]);
		std::swap(body->relativeVelocity[0], body->relativeVelocity[1]);

		LUCID_PROFILE_END();
	}

	void Simulator::evaluate(DynamicBody *body)
	{
		LUCID_PROFILE_BEGIN("simulating dynamic bodies");

		///	TBD: implement

		LUCID_PROFILE_END();
	}

	void Simulator::initialize()
	{
		shutdown();

		///	TBD: implement
	}

	void Simulator::shutdown()
	{
	}

	void Simulator::simulate(Frame *frame)
	{
		if (nullptr == frame)
			return;

		frame->apply(this);
		for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		{
			///	recursive example: sun <- earth <- moon <- apollo lander (arrow points to parent so, in this example, depth of 4) 
			simulate(child);
		}
	}

}	///	orbit
}	///	lucid
