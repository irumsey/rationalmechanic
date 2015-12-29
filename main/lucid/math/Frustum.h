#pragma once

#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Plane.h>

namespace lucid {
namespace math {

	///	Frustum
	///
	///	3D view frustum.
	template<class T> struct Frustum
	{
		enum PLANE
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

		Plane<T, 3> planes[PLANE_COUNT];
		Vector<T, 3> corners[CORNER_COUNT];

		Frustum() = default;

		Frustum(Matrix<T, 4, 4> const &invViewProj)
		{
			from(invViewProj);
		}

		void from(Matrix<T, 4, 4> const &invViewProj)
		{
			Vector<T, 3> const ppsCorners[] =
			{
				Vector<T, 3>(-1, -1, 0),
				Vector<T, 3>(-1,  1, 0),
				Vector<T, 3>( 1, -1, 0),
				Vector<T, 3>( 1,  1, 0),
				Vector<T, 3>(-1, -1, 1),
				Vector<T, 3>(-1,  1, 1),
				Vector<T, 3>( 1, -1, 1),
				Vector<T, 3>( 1,  1, 1),
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

}	///	math
}	///	lucid
