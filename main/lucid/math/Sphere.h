#pragma once

#include <lucid/math/Vector.h>

namespace lucid {
namespace math {

	///
	///
	///
	template<typename T, int DIM> struct Sphere
	{
		Vector<T, DIM> center;
		T radius = T();

		Sphere() = default;

		Sphere(Vector<T, DIM> const &center, T radius)
			: center(center)
			, radius(radius)
		{
		}

		~Sphere() = default;

	};

}	///	math
}	///	lucid
