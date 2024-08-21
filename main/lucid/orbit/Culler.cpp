#include "Culler.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/gal/System.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Profiler.h>

LUCID_ORBIT_BEGIN

Culler::Culler()
{
	// make sure SSB is not pruned (otherwise the scene will not render)
	_visibility[0].state = Visibility::STATE_CULLED;
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

void Culler::evaluate(OrbitalBody *body)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(OrbitalBody *)");

	static scalar_t const hysteresis[2] = { 0.01, 0.03, };

	Visibility &viz = _visibility[body->id];

	aabb3_t aabbTotal = aabb3_t(
		LUCID_MATH::lerp(_interpolant, body->aabbTotal[0].min, body->aabbTotal[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, body->aabbTotal[0].max, body->aabbTotal[1].max) - _cameraPosition
	);

	if (!LUCID_MATH::intersects(_frustum, aabbTotal))
	{
		viz.state = Visibility::STATE_PRUNED;
		return;
	}

	aabb3_t aabbBody = aabb3_t(
		LUCID_MATH::lerp(_interpolant, body->aabbSelf[0].min, body->aabbSelf[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, body->aabbSelf[0].max, body->aabbSelf[1].max) - _cameraPosition
	);

	if (!LUCID_MATH::intersects(_frustum, aabbBody))
	{
		viz.state = Visibility::STATE_CULLED;
		return;
	}

	PhysicalProperties const& physicalProperties = body->physicalProperties;

	viz.position = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	viz.rotation = LUCID_MATH::slerp(_interpolant, body->absoluteRotation[0], body->absoluteRotation[1]);
	viz.distance = LUCID_MATH::len(viz.position);
	viz.scaleFactor = physicalProperties.radius / viz.distance;

	scalar_t projRadius = ZNEAR * viz.scaleFactor;

	if (projRadius <= hysteresis[0])
		_visibility[body->id].state = Visibility::STATE_IMPERCEPTIBLE;
	else if ((hysteresis[0] < projRadius) && (projRadius <= hysteresis[1]) && (Visibility::STATE_VISIBLE != viz.state))
		_visibility[body->id].state = Visibility::STATE_IMPERCEPTIBLE;
	else
		_visibility[body->id].state = Visibility::STATE_VISIBLE;
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

	frame->apply(this);

	if (Visibility::STATE_PRUNED == _visibility[frame->id].state)
		return;

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		cull(child);
}

LUCID_ORBIT_END