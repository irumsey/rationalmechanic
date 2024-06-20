#pragma once

#include <lucid/units/System.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Plane.h>

#pragma push_macro("SCALAR")
#define SCALAR(T, S, Q) ::lucid::math::Scalar<T, ::lucid::units::Unit<S, Q> >

#pragma push_macro("VECTOR")
#define VECTOR(T, S, Q) ::lucid::math::Vector<T, 3, ::lucid::units::Unit<S, Q> >

#pragma push_macro("MATRIX")
#define MATRIX(T, S, Q) ::lucid::math::Matrix<T, 4, 4, ::lucid::units::Unit<S, Q> >

#pragma push_macro("PLANE")
#define PLANE(T, S, Q) ::lucid::math::Plane<T, 3, S, Q>

#pragma push_macro("FRUSTUM")
#define FRUSTUM(T, S, Q) ::lucid::math::Frustum<T, S, Q>

namespace lucid {
namespace math {

	///	Frustum
	///
	///	3D view frustum.
	template<typename T, typename S, typename Q> struct Frustum
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

		PLANE(T, S, Q) planes[PLANE_COUNT];
		VECTOR(T, S, Q) corners[CORNER_COUNT];

		Frustum() = default;

		Frustum(MATRIX(T, S, Q) const &invViewProj)
		{
			from(invViewProj);
		}

		~Frustum() = default;

		void from(MATRIX(T, S, Q) const &invViewProj)
		{
			VECTOR(T, S, Q) const ppsCorners[] =
			{
				VECTOR(T, S, Q)(-1, -1, 0),
				VECTOR(T, S, Q)(-1,  1, 0),
				VECTOR(T, S, Q)( 1, -1, 0),
				VECTOR(T, S, Q)( 1,  1, 0),
				VECTOR(T, S, Q)(-1, -1, 1),
				VECTOR(T, S, Q)(-1,  1, 1),
				VECTOR(T, S, Q)( 1, -1, 1),
				VECTOR(T, S, Q)( 1,  1, 1),
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

	///
	///
	///
	
	template<typename T, typename S, typename Q>
	inline FRUSTUM(T, S, Q) makeFrustum3(MATRIX(T, S, Q) const &invViewProj)
	{
		return FRUSTUM(T, S, Q)(invViewProj);
	}

}	///	math
}	///	lucid

///
///
///

#undef FRUSTUM
#pragma pop_macro("FRUSTUM")

#undef PLANE
#pragma pop_macro("PLANE")

#undef MATRIX
#pragma pop_macro("MATRIX")

#undef VECTOR
#pragma pop_macro("VECTOR")

#undef SCALAR
#pragma pop_macro("SCALAR")
