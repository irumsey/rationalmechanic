#pragma once

#include <unordered_map>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Algorithm.h>

LUCID_ORBIT_BEGIN

class Frame;
class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class CameraFrame;

///	Culler
/// 
///	Used to traverse the scene and cull/prune branches based upon visibility.
/// At the same time, Culler fits the view volume to that which is visible
/// and determines appropriate scaling factors for rendering.
class Culler : public Algorithm
{
public:
	enum STATE {
		STATE_PRUNED = 0,		// whole branch not in view volume
		STATE_CULLED,			// just the parent frame not in view volume
		STATE_IMPERCEPTIBLE,	// too small to render
		STATE_VISIBLE,			// frame is visible
	};

	scalar_t znear;
	scalar_t zfar;

	scalar_t sceneScalingFactor;
	scalar_t starFieldRadius;
	scalar_t starScalingFactor;

	Culler();

	virtual ~Culler();

	void cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant);

	STATE operator[](size_t id);

	STATE frameState(size_t id);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	friend class Renderer;

	///	Visibility
	/// 
	///	Needed to provide a default visibility state when looking up
	/// an id using an unordered_map<>
	struct Visibility
	{
		STATE state = STATE_PRUNED;

		Visibility() = default;

		Visibility(Visibility const &) = default;

		Visibility(STATE state)
			: state(state)
		{
		}

		~Visibility() = default;

		Visibility &operator =(Visibility const &) = default;

	};

	scalar_t const ZNEAR_INITIAL = 10.0;
	scalar_t const  ZFAR_INITIAL = 39.23547229976602 * constants::meters_per_AU<float64_t>;

	scalar_t _interpolant;

	matrix4x4_t _projMatrix;
	matrix4x4_t _viewMatrix;
	matrix4x4_t _viewProjMatrix;
	matrix4x4_t _invViewProjMatrix;

	frustum_t _frustum;

	std::unordered_map<size_t, Visibility> _visibility;

	void cull(Frame *frame);

};

inline Culler::STATE Culler::operator[](size_t id)
{
	return frameState(id);
}

inline Culler::STATE Culler::frameState(size_t id)
{
	return _visibility[id].state;
}

LUCID_ORBIT_END