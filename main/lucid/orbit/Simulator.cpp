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

	void Simulator::evaluate(DynamicPoint *frame)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(DynamicPoint)");
		basicEvaluation(frame);
	}

	void Simulator::evaluate(OrbitalBody *body)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(OrbitalBody)");

		Frame const *center = body->centerFrame;
		LUCID_VALIDATE(body != center, "orbital body is defined as root (it has no center, attracting body, defined)");

		body->elements[0] = body->elements[1];
		theEphemeris().lookup(body->elements[1], body->id, _dayNumber);

		PhysicalProperties centerProperties;
		theEphemeris().lookup(centerProperties, center->id);

		body->relativePosition[0] = body->relativePosition[1];
		body->relativeVelocity[0] = body->relativeVelocity[1];
		kinematicsFromElements(body->relativePosition[1], body->relativeVelocity[1], centerProperties, body->elements[1], _dayNumber);

		body->absolutePosition[0] = body->absolutePosition[1];
		body->absolutePosition[1] = body->relativePosition[1] + center->absolutePosition[1];

		PhysicalProperties const &bodyProperties = body->physicalProperties;
		scalar_t radius = bodyProperties.radius;

		vector3_t extents = vector3_t(radius, radius, radius);

		body->aabb[0] = body->aabb[1];
		body->aabb[1] = aabb3_t(body->absolutePosition[1] - extents, body->absolutePosition[1] + extents);
	}

	void Simulator::evaluate(DynamicBody *body)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(DynamicBody)");
		///	TBD: implement (this will be complex)
	}

	void Simulator::evaluate(CameraFrame *camera)
	{
		LUCID_PROFILE_SCOPE("Simulator::evaluate(CameraFrame)");
		basicEvaluation(camera);
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

			/// child has expanded itself to contain its children (using this recursion), now fit it
			frame->aabb[1] = math::fit(frame->aabb[1], child->aabb[1]);
		}
	}

	void Simulator::basicEvaluation(Frame *frame)
	{
		Frame const *center = frame->centerFrame;
		LUCID_VALIDATE(nullptr != center, "internal consistency error: detached frame in simulation");

		// an axis aligned box centered at the position with zero volume
		frame->aabb[0] = frame->aabb[1];
		frame->aabb[1] = aabb3_t(frame->absolutePosition[1], frame->absolutePosition[1]);

		// if this is a "root" frame, don't update positions (keep them as originally set).
		if (frame == center)
			return;

		frame->relativePosition[0] = frame->relativePosition[1];
		frame->absolutePosition[0] = frame->absolutePosition[1];
		frame->absolutePosition[1] = center->absolutePosition[1] + frame->relativePosition[1];
	}

}	///	orbit
}	///	lucid
