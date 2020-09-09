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
		LUCID_PROFILE_SCOPE("Simulator::evaluate(DynamicPoint)");

		///	relative position 1 can be changed externally
		point->relativePosition[0] = point->relativePosition[1];

		Frame const *center = point->centerFrame;
		if (nullptr == center)
			return;

		point->absolutePosition[0] = point->absolutePosition[1];
		point->absolutePosition[1] = point->relativePosition[1] + center->absolutePosition[1];
	}

	void Simulator::evaluate(OrbitalBody *body)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(OrbitalBody)");

		Frame const *center = body->centerFrame;
		LUCID_VALIDATE(nullptr != center, "orbital body has no center, attracting body, defined");

		body->elements[0] = body->elements[1];
		theEphemeris().lookup(body->elements[1], body->id, _dayNumber);

		PhysicalProperties centerProperties;
		theEphemeris().lookup(centerProperties, center->id);

		body->relativePosition[0] = body->relativePosition[1];
		body->relativeVelocity[0] = body->relativeVelocity[1];
		kinematicsFromElements(body->relativePosition[1], body->relativeVelocity[1], centerProperties, body->elements[1], _dayNumber);

		body->absolutePosition[0] = body->absolutePosition[1];
		body->absolutePosition[1] = body->relativePosition[1] + center->absolutePosition[1];
	}

	void Simulator::evaluate(DynamicBody *body)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(DynamicBody)");

		///	TBD: implement
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
