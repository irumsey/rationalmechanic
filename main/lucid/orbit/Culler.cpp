#include "Culler.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/gal/System.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Profiler.h>

namespace lucid {
namespace orbit {

	Culler::Culler()
	{
		// set the SSB (solar system barycenter) to visible
		_visibility[0] = STATE_VISIBLE;
	}

	Culler::~Culler()
	{
	}

	void Culler::cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant)
	{
		LUCID_PROFILE_SCOPE("Culler::cull()");

		gal::System const &galSystem = gal::System::instance();

		scalar_t screenWidth = scalar_t(galSystem.width());
		scalar_t screenHeight = scalar_t(galSystem.height());
		scalar_t const aspect = scalar_t(galSystem.aspect());

		scalar_t const fov = cameraFrame->fov;

		_interpolant = interpolant;

		znear = ZNEAR_INITIAL;
		zfar = ZFAR_INITIAL;

		vector3_t cameraPosition = math::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
		vector3_t focusPosition = math::lerp(interpolant, cameraFrame->focus->absolutePosition[0], cameraFrame->focus->absolutePosition[1]);

		_projMatrix = math::perspective(fov, aspect, znear, zfar);
		_viewMatrix = math::look(cameraPosition, focusPosition, vector3_t(0, 0, 1));
		_viewProjMatrix = _projMatrix * _viewMatrix;
		_invViewProjMatrix = math::inverse(_viewProjMatrix);

		_frustum = math::makeFrustum3(_invViewProjMatrix);

		std::swap(znear, zfar);

		cull(rootFrame);

		znear = math::min(znear, zfar * math::cos(fov));

		_projMatrix = math::perspective(fov, aspect, znear, zfar);
		_viewMatrix = math::look(vector3_t(0, 0, 0), math::normalize(focusPosition - cameraPosition), vector3_t(0, 0, 1));
		_viewProjMatrix = _projMatrix * _viewMatrix;
		_invViewProjMatrix = math::inverse(_viewProjMatrix);
		
		vector4_t sprite[3] = {
			vector4_t(               0.0,                 0.0, 1.0, 1.0),
			vector4_t(-1.0 / screenWidth, -1.0 / screenHeight, 1.0, 1.0),
			vector4_t( 1.0 / screenWidth,  1.0 / screenHeight, 1.0, 1.0),
		};
		
		for (size_t i = 0; i < 3; ++i)
		{
			sprite[i] = _invViewProjMatrix * sprite[i];
			sprite[i] = sprite[i] / scalar_t(sprite[i].w);
		}

		starFieldRadius = math::len(vector3_t(sprite[0].x, sprite[0].y, sprite[0].z));
		starScalingFactor = math::len(vector3_t(sprite[2].x, sprite[2].y, sprite[2].z) - vector3_t(sprite[1].x, sprite[1].y, sprite[1].z));

		sceneScalingFactor = 1.0 / (zfar - znear);
	}

	void Culler::evaluate(DynamicPoint *point)
	{
		LUCID_PROFILE_SCOPE("Culler::evaluate(DynamicPoint *)");
	}

	void Culler::evaluate(OrbitalBody *body)
	{
		LUCID_PROFILE_SCOPE("Culler::evaluate(OrbitalBody *)");

		static scalar_t const hysteresis[2] = { 0.0001, 0.0003, };

		aabb3_t aabbTotal = aabb3_t(
			math::lerp(_interpolant, body->aabbTotal[0].min, body->aabbTotal[1].min),
			math::lerp(_interpolant, body->aabbTotal[0].max, body->aabbTotal[1].max)
		);

		if (!math::intersects(_frustum, aabbTotal))
		{
			_visibility[body->id] = STATE_PRUNED;
			return;
		}

		aabb3_t aabbBody = aabb3_t(
			math::lerp(_interpolant, body->aabbSelf[0].min, body->aabbSelf[1].min),
			math::lerp(_interpolant, body->aabbSelf[0].max, body->aabbSelf[1].max)
		);

		if (!math::intersects(_frustum, aabbBody))
		{
			_visibility[body->id] = STATE_CULLED;
			return;
		}

		scalar_t area = math::areaProjected(_viewProjMatrix, ZNEAR_INITIAL, aabbBody);

		if (area <= hysteresis[0])
			_visibility[body->id] = STATE_IMPERCEPTIBLE;
		else if ((hysteresis[0] < area) && (area <= hysteresis[1]) && (STATE_VISIBLE != frameState(body->id)))
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
			scalar_t depth = math::dot(_frustum.planes[frustum_t::PLANE_NEAR], corners[i]);

			znear = (depth > 0) ? math::min(depth, znear) : znear;
			zfar = (depth > 0) ? math::max(depth, zfar) : zfar;
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

}	/// orbit
}	///	lucid