#include "Simulator.h"
#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/core/Logger.h>

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

LUCID_ORBIT_BEGIN

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

	PhysicalProperties const &properties = body->physicalProperties;
	scalar_t radius = properties.radius;
	scalar_t GM = properties.GM;

	Frame const *center = body->centerFrame;
	LUCID_VALIDATE(nullptr != center, "consistency error: " + body->name + " does not have a center frame defined");

	body->elements[0] = body->elements[1];
	if (!LUCID_ORBIT_EPHEMERIS.lookup(body->elements[1], body->id, _dayNumber))
	{
		LUCID_CORE::log("ERR", "specified JDN out-of-bounds (orbital elements) for: " + body->name);
		body->simState = Frame::SIM_STATE_ERROR;
		return;
	}

	RotationalElements rotationalElements;
	if (!LUCID_ORBIT_EPHEMERIS.lookup(rotationalElements, body->id, _dayNumber))
	{
		LUCID_CORE::log("ERR", "specified JDN out-of-bounds (rotational elements) for: " + body->name);
		body->simState = Frame::SIM_STATE_ERROR;
		return;
	}

	Frame const *centerFrame = body->centerFrame;
	PhysicalProperties centerProperties;
	if (LUCID_ORBIT_EPHEMERIS.lookup(centerProperties, centerFrame->id))
	{
		body->relativePosition[0] = body->relativePosition[1];
		body->relativeVelocity[0] = body->relativeVelocity[1];
		kinematicsFromElements(body->relativePosition[1], body->relativeVelocity[1], body->elements[1], centerProperties, _dayNumber);

		body->absolutePosition[0] = body->absolutePosition[1];
		body->absolutePosition[1] = body->relativePosition[1] + center->absolutePosition[1];

		body->absoluteVelocity[0] = body->absoluteVelocity[1];
		body->absoluteVelocity[1] = body->relativeVelocity[1] + center->absoluteVelocity[1];
	}

	body->relativeRotation[0] = body->relativeRotation[1];
	rotationFromElements(body->relativeRotation[1], rotationalElements, _dayNumber);

	// TBD: keep absolute == relative...
	body->absoluteRotation[0] = body->absoluteRotation[1];
	body->absoluteRotation[1] = body->relativeRotation[1];

	vector3_t extents = vector3_t(radius, radius, radius);

	body->aabbSelf[0] = body->aabbSelf[1];
	body->aabbSelf[1] = aabb3_t(body->absolutePosition[1] - extents, body->absolutePosition[1] + extents);

	body->aabbTotal[0] = body->aabbTotal[1];
	body->aabbTotal[1] = body->aabbSelf[1];

	body->simState = Frame::SIM_STATE_STABLE;
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

	if (Frame::SIM_STATE_ERROR == frame->simState)
		return;

	frame->apply(this);
	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
	{
		///	recursive example: sun <- earth <- moon <- apollo lander (arrow points to parent so, in this example, depth of 4) 
		simulate(child);

		LUCID_MATH::fit(frame->aabbTotal[1], child->aabbTotal[1]);
	}
}

void Simulator::basicEvaluation(Frame *frame)
{
	Frame const *center = frame->centerFrame;
	LUCID_VALIDATE(nullptr != center, "internal consistency error: detached frame in simulation");

	// an axis aligned box centered at the position with zero volume
	frame->aabbSelf[0] = frame->aabbSelf[1];
	frame->aabbSelf[1] = aabb3_t(frame->absolutePosition[1], frame->absolutePosition[1]);

	frame->aabbTotal[0] = frame->aabbTotal[1];
	frame->aabbTotal[1] = frame->aabbSelf[1];

	// if this is a "root" frame, don't update positions (keep them as originally set).
	if (frame == center)
		return;

	frame->relativePosition[0] = frame->relativePosition[1];
	frame->absolutePosition[0] = frame->absolutePosition[1];
	frame->absolutePosition[1] = center->absolutePosition[1] + frame->relativePosition[1];
}

LUCID_ORBIT_END
