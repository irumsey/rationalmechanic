#pragma once

#include <rnd/_orbit/Defines.h>

ORBIT_BEGIN

class DynamicPoint;
class OrbitalBody;
class DynamicBody;
class Camera;

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

	virtual void evaluate(Camera *camera) = 0;

protected:
	Algorithm() = default;

};

ORBIT_END