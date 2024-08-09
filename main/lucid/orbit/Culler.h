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
	///	Visibility
	/// 
	/// 
	struct Visibility
	{
		enum STATE {
			STATE_PRUNED = 0,		// whole branch, this frame and children, not in view volume
			STATE_CULLED,			// just this, the parent frame, not in view volume
			STATE_IMPERCEPTIBLE,	// too small to render
			STATE_VISIBLE,			// frame is visible
		};

		STATE           state = STATE_PRUNED;

		vector3_t    position;		// position relative to camera
		scalar_t     distance = 0;	// distance from camera
		scalar_t  scaleFactor = 0;	// ratio of object radius and distance
	};

	Culler();

	virtual ~Culler();

	void cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant);

	Visibility const &operator[](size_t id);

	Visibility const &frameState(size_t id);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	scalar_t const ZNEAR = 10.0;
	scalar_t const  ZFAR = 50.0 * constants::meters_per_AU<scalar_t>;

	scalar_t _interpolant = 0;

	vector3_t _cameraPosition;
	frustum_t _frustum;

	std::unordered_map<size_t, Visibility> _visibility;

	void cull(Frame *frame);

};

inline Culler::Visibility const &Culler::operator[](size_t id)
{
	return frameState(id);
}

inline Culler::Visibility const &Culler::frameState(size_t id)
{
	return _visibility[id];
}

LUCID_ORBIT_END