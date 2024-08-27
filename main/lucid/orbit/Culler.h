#pragma once

#include <lucid/core/Types.h>
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
class Culler : public Algorithm
{
public:
	Culler();

	virtual ~Culler();

	void cull(Frame *rootFrame, CameraFrame *cameraFrame, scalar_t const &interpolant);

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

private:
	///	test {
	/// magic numbers
	scalar_t const ZNEAR = 10.0;
	scalar_t const  ZFAR = 50.0 * constants::meters_per_AU<scalar_t>;
	scalar_t const hysteresis[2] = { 0.01, 0.03, };
	/// }

	scalar_t _interpolant = 0;
	vector3_t _cameraPosition;
	frustum_t _frustum;

	void cull(Frame *frame);
};

LUCID_ORBIT_END