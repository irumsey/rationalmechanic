#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Plane.h>

LUCID_MATH_BEGIN

///	Frustum
///
///	3D view frustum.
template<typename T> struct Frustum
{
	enum LUCID_PLANE
	{
		PLANE_NEAR = 0,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_TOP,
		PLANE_BOTTOM,
	};

	enum { PLANE_COUNT = 6 };
	enum { CORNER_COUNT = 8 };

	LUCID_PLANE(T, 3) planes[PLANE_COUNT];
	LUCID_VECTOR(T, 3) corners[CORNER_COUNT];

	Frustum() = default;

	Frustum(LUCID_MATRIX(T, 4, 4) const &invViewProj)
	{
		from(invViewProj);
	}

	~Frustum() = default;

	void from(LUCID_MATRIX(T, 4, 4) const &invViewProj)
	{
		LUCID_VECTOR(T, 3) const ppsCorners[] =
		{
			LUCID_VECTOR(T, 3)(-1, -1, 0),
			LUCID_VECTOR(T, 3)(-1,  1, 0),
			LUCID_VECTOR(T, 3)( 1, -1, 0),
			LUCID_VECTOR(T, 3)( 1,  1, 0),
			LUCID_VECTOR(T, 3)(-1, -1, 1),
			LUCID_VECTOR(T, 3)(-1,  1, 1),
			LUCID_VECTOR(T, 3)( 1, -1, 1),
			LUCID_VECTOR(T, 3)( 1,  1, 1),
		};

		for (int i = 0; i < CORNER_COUNT; ++i)
		{
			corners[i] = transform(invViewProj, ppsCorners[i]);
		}

		planes[  PLANE_NEAR] = makePlane3(corners[0], corners[1], corners[3]);
		planes[   PLANE_FAR] = makePlane3(corners[5], corners[6], corners[7]);
		planes[  PLANE_LEFT] = makePlane3(corners[1], corners[4], corners[5]);
		planes[ PLANE_RIGHT] = makePlane3(corners[2], corners[3], corners[6]);
		planes[   PLANE_TOP] = makePlane3(corners[3], corners[5], corners[7]);
		planes[PLANE_BOTTOM] = makePlane3(corners[0], corners[2], corners[4]);
	}
};
	
template<typename T> inline LUCID_FRUSTUM(T) makeFrustum3(LUCID_MATRIX(T, 4, 4) const &invViewProj)
{
	return LUCID_FRUSTUM(T)(invViewProj);
}

LUCID_MATH_END

