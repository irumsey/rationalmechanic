#pragma once

#include <lucid/orbit/Defines.h>

LUCID_ORBIT_BEGIN

class Frame;
class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class CameraFrame;

///
///
///
class Algorithm
{
public:
	virtual ~Algorithm() = default;

	virtual void evaluate(DynamicPoint *point) = 0;

	virtual void evaluate(OrbitalBody *body) = 0;

	virtual void evaluate(DynamicBody *body) = 0;

	virtual void evaluate(CameraFrame *camera) = 0;

protected:
	Algorithm() = default;

};

LUCID_ORBIT_END