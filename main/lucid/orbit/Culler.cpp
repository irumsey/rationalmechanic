#include "Culler.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/gal/System.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Profiler.h>

LUCID_ORBIT_BEGIN

Culler::Culler()
{
}

Culler::~Culler()
{
}

void Culler::cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant)
{
	LUCID_PROFILE_SCOPE("Culler::cull()");

	scalar_t const fov = cameraFrame->fov;
	scalar_t const aspect = scalar_t(LUCID_GAL_SYSTEM.aspect());

	_interpolant = interpolant;

	_cameraPosition = LUCID_MATH::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
	vector3_t focusPosition = LUCID_MATH::lerp(interpolant, cameraFrame->focus->absolutePosition[0], cameraFrame->focus->absolutePosition[1]);

	matrix4x4_t projMatrix = LUCID_MATH::perspective(fov, aspect, ZNEAR, ZFAR);
	matrix4x4_t viewMatrix = LUCID_MATH::look(vector3_t(0, 0, 0), (focusPosition - _cameraPosition), vector3_t(0, 0, 1));

	_frustum = LUCID_MATH::makeFrustum3(LUCID_MATH::inverse(projMatrix * viewMatrix));

	cull(rootFrame);
}

void Culler::evaluate(DynamicPoint *point)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(DynamicPoint *)");
}

/// hysteresis in this context simply means the projected radius has to go below
/// h0 to become imperceptible and above h1 to become visible.  between the two
/// values (h0 and h1) the visiblity state does not change.
void Culler::evaluate(OrbitalBody *body)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(OrbitalBody *)");

	PhysicalProperties const &physicalProperties = body->physicalProperties;

	vector3_t position = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	scalar_t distance = LUCID_MATH::len(position);
	scalar_t scaleFactor = physicalProperties.radius / distance;

	// preserve the visible state or force to imperceptible if previously culled or pruned. 
	body->cullState = (Frame::CULL_STATE_VISIBLE == body->cullState)
		? Frame::CULL_STATE_VISIBLE
		: Frame::CULL_STATE_IMPERCEPTIBLE;

	scalar_t projRadius = ZNEAR * scaleFactor;
	if (projRadius <= hysteresis[0])
		body->cullState = Frame::CULL_STATE_IMPERCEPTIBLE;
	else if (hysteresis[1] < projRadius)
		body->cullState = Frame::CULL_STATE_VISIBLE;
}

void Culler::evaluate(DynamicBody *body)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(DynamicBody *)");
}

void Culler::evaluate(CameraFrame *camera)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(CameraFrame *)");
}

void Culler::cull(Frame *frame)
{
	LUCID_PROFILE_SCOPE("Culler::cull(Frame *)");

	if (Frame::SIM_STATE_ERROR == frame->simState)
		return;

	aabb3_t aabbTotal = aabb3_t(
		LUCID_MATH::lerp(_interpolant, frame->aabbTotal[0].min, frame->aabbTotal[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, frame->aabbTotal[0].max, frame->aabbTotal[1].max) - _cameraPosition
	);

	if (!LUCID_MATH::intersects(_frustum, aabbTotal))
	{
		frame->cullState = Frame::CULL_STATE_PRUNED;
		return;
	}

	aabb3_t aabbSelf = aabb3_t(
		LUCID_MATH::lerp(_interpolant, frame->aabbSelf[0].min, frame->aabbSelf[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, frame->aabbSelf[0].max, frame->aabbSelf[1].max) - _cameraPosition
	);

	if (LUCID_MATH::intersects(_frustum, aabbSelf))
		frame->apply(this);
	else
		frame->cullState = Frame::CULL_STATE_CULLED;

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		cull(child);
}

LUCID_ORBIT_END