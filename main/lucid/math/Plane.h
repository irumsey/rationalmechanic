#pragma once

#include <lucid/units/System.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

LUCID_MATH_BEGIN

///	Plane
///
///	defines two "half spaces"
template<typename T, size_t DIM, typename S, typename Q> struct Plane
{
	LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_PURE) n;
	LUCID_SCALAR(T, S, Q) d;

	Plane() = default;

	Plane(LUCID_VECTOR(T, DIM, S, LUCID_QUANTITY_PURE) const &n, LUCID_SCALAR(T, S, Q) const &d)
		: n(n)
		, d(d)
	{
	}

	~Plane() = default;

};

template<typename T, size_t DIM, typename S, typename Q>
inline LUCID_SCALAR(T, S, Q) dot(LUCID_PLANE(T, DIM, S, Q) const &lhs, LUCID_VECTOR(T, DIM, S, Q) const &rhs)
{
	return dot(lhs.n, rhs) + lhs.d;
}

template<typename T, typename S, typename Q>
inline LUCID_PLANE(T, 3, S, Q) makePlane3(LUCID_VECTOR(T, 3, S, Q) const &a, LUCID_VECTOR(T, 3, S, Q) const &b, LUCID_VECTOR(T, 3, S, Q) const &c)
{
	LUCID_VECTOR(T, 3, S, LUCID_QUANTITY_PURE) normal = normalize(cross(b - a, c - a));
	return LUCID_PLANE(T, 3, S, Q)(normal, -dot(normal, a));
}

LUCID_MATH_END
