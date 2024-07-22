#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///	Plane
///
///	defines two "half spaces"
template<typename T, size_t DIM> struct Plane
{
	LUCID_VECTOR(T, DIM) n;
	T d;

	Plane() = default;

	Plane(LUCID_VECTOR(T, DIM) const &n, T const &d)
		: n(n)
		, d(d)
	{
	}

	~Plane() = default;

};

template<typename T, size_t DIM> inline T dot(LUCID_PLANE(T, DIM) const &lhs, LUCID_VECTOR(T, DIM) const &rhs)
{
	return dot(lhs.n, rhs) + lhs.d;
}

template<typename T> inline LUCID_PLANE(T, 3) makePlane3(LUCID_VECTOR(T, 3) const &a, LUCID_VECTOR(T, 3) const &b, LUCID_VECTOR(T, 3) const &c)
{
	LUCID_VECTOR(T, 3) normal = normalize(cross(b - a, c - a));
	return LUCID_PLANE(T, 3)(normal, -dot(normal, a));
}

LUCID_MATH_END
