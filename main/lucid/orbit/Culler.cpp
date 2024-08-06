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
	_visibility[0] = STATE_IMPERCEPTIBLE;
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

	_projMatrix = LUCID_MATH::perspective(fov, aspect, ZNEAR_INITIAL, ZFAR_INITIAL);
	_viewMatrix = LUCID_MATH::look(vector3_t(0, 0, 0), (focusPosition - _cameraPosition), vector3_t(0, 0, 1));
	_viewProjMatrix = _projMatrix * _viewMatrix;
	_invViewProjMatrix = LUCID_MATH::inverse(_viewProjMatrix);

	_frustum = LUCID_MATH::makeFrustum3(_invViewProjMatrix);

	znear = ZFAR_INITIAL;
	zfar = ZNEAR_INITIAL;

	cull(rootFrame);

	sceneScaleFactor = 1.0 / (zfar - znear);
}

void Culler::evaluate(DynamicPoint *point)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(DynamicPoint *)");
}

void Culler::evaluate(OrbitalBody *body)
{
	LUCID_PROFILE_SCOPE("Culler::evaluate(OrbitalBody *)");

	static scalar_t const hysteresis[2] = { 0.1, 0.3, };

	aabb3_t aabbTotal = aabb3_t(
		LUCID_MATH::lerp(_interpolant, body->aabbTotal[0].min, body->aabbTotal[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, body->aabbTotal[0].max, body->aabbTotal[1].max) - _cameraPosition
	);

	if (!LUCID_MATH::intersects(_frustum, aabbTotal))
	{
		_visibility[body->id] = STATE_PRUNED;
		return;
	}

	aabb3_t aabbBody = aabb3_t(
		LUCID_MATH::lerp(_interpolant, body->aabbSelf[0].min, body->aabbSelf[1].min) - _cameraPosition,
		LUCID_MATH::lerp(_interpolant, body->aabbSelf[0].max, body->aabbSelf[1].max) - _cameraPosition
	);

	if (!LUCID_MATH::intersects(_frustum, aabbBody))
	{
		_visibility[body->id] = STATE_CULLED;
		return;
	}

	PhysicalProperties const& physicalProperties = body->physicalProperties;

	vector3_t bodyPosition = LUCID_MATH::lerp(_interpolant, body->absolutePosition[0], body->absolutePosition[1]) - _cameraPosition;
	scalar_t bodyDistance = LUCID_MATH::len(bodyPosition);
	scalar_t radius = ZNEAR_INITIAL * physicalProperties.radius / bodyDistance;

	if (radius <= hysteresis[0])
		_visibility[body->id] = STATE_IMPERCEPTIBLE;
	else if ((hysteresis[0] < radius) && (radius <= hysteresis[1]) && (STATE_VISIBLE != frameState(body->id)))
		_visibility[body->id] = STATE_IMPERCEPTIBLE;
	else
		_visibility[body->id] = STATE_VISIBLE;

	if (STATE_VISIBLE != frameState(body->id))
		return;

	vector3_t const corners[] =
	{
		vector3_t(aabbBody.min.x, aabbBody.min.y, aabbBody.min.z),
		vector3_t(aabbBody.min.x, aabbBody.min.y, aabbBody.max.z),
		vector3_t(aabbBody.min.x, aabbBody.max.y, aabbBody.min.z),
		vector3_t(aabbBody.min.x, aabbBody.max.y, aabbBody.max.z),
		vector3_t(aabbBody.max.x, aabbBody.min.y, aabbBody.min.z),
		vector3_t(aabbBody.max.x, aabbBody.min.y, aabbBody.max.z),
		vector3_t(aabbBody.max.x, aabbBody.max.y, aabbBody.min.z),
		vector3_t(aabbBody.max.x, aabbBody.max.y, aabbBody.max.z),
	};

	for (size_t i = 0; i < 8; ++i)
	{
		scalar_t depth = LUCID_MATH::dot(_frustum.planes[frustum_t::PLANE_NEAR], corners[i]);
		if (depth > 0.0)
		{
			znear = LUCID_MATH::min(depth, znear);
			zfar = LUCID_MATH::max(depth, zfar);
		}
	}
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

	if (STATE_PRUNED == frameState(frame->id))
		return;

	for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
		cull(child);
}

LUCID_ORBIT_END