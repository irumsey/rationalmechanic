#include "Culler.h"
#include "Frame.h"
#include "Utility.h"
#include <lucid/gal/System.h>
#include <lucid/math/Algorithm.h>

namespace lucid {
namespace orbit {

	Culler::Culler()
	{
		// make the SSB (solar system barycenter) to visible
		visibility[0] = Visibility::STATE_VISIBLE;
	}

	Culler::~Culler()
	{
	}

	void Culler::cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant)
	{
		gal::System const &galSystem = gal::System::instance();
		scalar_t screenWidth = float64_t(galSystem.width());
		scalar_t screenHeight = float64_t(galSystem.height());

		_interpolant = interpolant;

		scalar_t aspect = screenWidth / screenHeight;
		znear = znearInitial;
		zfar = 39.23547229976602 * constants::meters_per_AU<float64_t>;

		vector3_t cameraPosition = math::lerp(interpolant, cameraFrame->absolutePosition[0], cameraFrame->absolutePosition[1]);
		vector3_t focusPosition = math::lerp(interpolant, cameraFrame->focus->absolutePosition[0], cameraFrame->focus->absolutePosition[1]);

		projMatrix = math::perspective(scalar_t(0.25 * constants::pi<float64_t>), aspect, znear, zfar);
		viewMatrix = math::look(cameraPosition, focusPosition, vector3_t(0, 0, 1));
		_viewProjMatrix = projMatrix * viewMatrix;

		_frustum = math::makeFrustum3(math::inverse(_viewProjMatrix));

		std::swap(znear, zfar);

		cull(rootFrame);

		scaleFactor = 500.0 / (zfar - znear);

		projMatrix = math::perspective(scalar_t(0.25 * constants::pi<float64_t>), aspect, scaleFactor * znear, scaleFactor * zfar);
		viewMatrix = math::look(vector3_t(0, 0, 0), math::normalize(focusPosition - cameraPosition), vector3_t(0, 0, 1));
	}

	void Culler::evaluate(DynamicPoint *point)
	{
	}

	void Culler::evaluate(OrbitalBody *body)
	{
		static scalar_t const hysteresis[2] = { 0.0001, 0.0003, };

		aabb3_t aabbTotal = aabb3_t(
			math::lerp(_interpolant, body->aabbTotal[0].min, body->aabbTotal[1].min),
			math::lerp(_interpolant, body->aabbTotal[0].max, body->aabbTotal[1].max)
		);

		if (!math::intersects(_frustum, aabbTotal))
		{
			visibility[body->id] = Visibility::STATE_PRUNED;
			return;
		}

		aabb3_t aabbBody = aabb3_t(
			math::lerp(_interpolant, body->aabbSelf[0].min, body->aabbSelf[1].min),
			math::lerp(_interpolant, body->aabbSelf[0].max, body->aabbSelf[1].max)
		);

		if (!math::intersects(_frustum, aabbBody))
		{
			visibility[body->id] = Visibility::STATE_CULLED;
			return;
		}

		scalar_t area = math::areaProjected(_viewProjMatrix, znearInitial, aabbBody);

		if (area <= hysteresis[0])
			visibility[body->id] = Visibility::STATE_IMPERCEPTIBLE;
		else if ((hysteresis[0] < area) && (area <= hysteresis[1]) && (Visibility::STATE_VISIBLE != visibility[body->id].state))
			visibility[body->id] = Visibility::STATE_IMPERCEPTIBLE;
		else
			visibility[body->id] = Visibility::STATE_VISIBLE;

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

	}

	void Culler::evaluate(CameraFrame *camera)
	{
	}

	void Culler::cull(Frame *frame)
	{
		frame->apply(this);

		if (Visibility::STATE_PRUNED == visibility[frame->id].state)
			return;

		for (Frame *child = frame->firstChild; nullptr != child; child = child->nextSibling)
			cull(child);
	}

}	/// orbit
}	///	lucid