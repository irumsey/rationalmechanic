#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///
///
///
template<typename T, size_t DIM>
struct Sphere
{
	Vector<T, DIM> center;
	T radius;

	Sphere() = default;

	Sphere(Vector<T, DIM> const &center, T const &radius)
		: center(center)
		, radius(radius)
	{
	}

	~Sphere() = default;

};

LUCID_MATH_END