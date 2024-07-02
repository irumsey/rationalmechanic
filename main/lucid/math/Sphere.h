#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///
///
///
template<typename T, size_t DIM, typename U>
struct Sphere
{
	Vector<T, DIM, U> center;
	Scalar<T, U> radius;

	Sphere() = default;

	Sphere(Vector<T, DIM, U> const &center, Scalar<T, U> const &radius)
		: center(center)
		, radius(radius)
	{
	}

	~Sphere() = default;

};

LUCID_MATH_END