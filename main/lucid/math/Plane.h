#pragma once

#include <lucid/math/Vector.h>

namespace lucid {
namespace math {

	///	Plane
	///
	///	defines two "half spaces"
	template<class T, int DIM> struct Plane
	{
		Vector<T,DIM> n;
		T d = T();

		Plane() = default;

		Plane(Vector<T,DIM> const &n, T const &d)
			: n(n)
			, d(d)
		{
		}

	};

}	///	math
}	///	lucid

///	TBD: operators...

namespace lucid {
namespace math {

	///	dot
	///
	///	dot product
	template<class T, int DIM> inline T dot(Plane<T, DIM> const &lhs, Vector<T, DIM> const &rhs)
	{
		return dot(lhs.n, rhs) + lhs.d;
	}

	template<class T> inline Plane<T, 3> makePlane3(Vector<T, 3> const &a, Vector<T, 3> const &b, Vector<T, 3> const &c)
	{
		Vector<T, 3> normal = normalize(cross(b - a, c - a));
		return Plane<T, 3>(normal, -dot(normal, a));
	}

}	///	math
}	///	lucid
