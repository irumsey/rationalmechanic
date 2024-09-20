#pragma once

#include <algorithm>
#include <lucid/core/Numbers.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Integer.h>
#include <lucid/math/Fixed.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Sphere.h>
#include <lucid/math/Plane.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>

#ifdef min
#	undef min
#	undef max
#endif

LUCID_MATH_BEGIN

///
/// 
/// 

template<typename T> inline T sign(T const &x)
{
	return (x < 0) ? T(-1) : T(1);
}

///
/// 
/// 

template<typename T> inline T abs(T const &x)
{
	return T(std::abs(x));
}

/// min / max
///
///

template<typename T> inline T min(T const &x, T const &y)
{
	return (x <= y) ? x : y;
}

template<typename T> inline T max(T const &x, T const &y)
{
	return (x >= y) ? x : y;
}

///	interpolate
///
///	linear interpolation from a to b.
template<typename T, typename U> inline U lerp(T const &t, U const &a, U const &b)
{
	return (b - a) * t + a;
}

///	clamp
///
///	clamp value between values a and b
///	where a < b
template<typename T> inline T clamp(T const &x, T const &a, T const &b)
{
	return std::min(std::max(a, x), b);
}

///	exclude
///
///	range of exclusion for the value x defined by a and b
///	where a < b
template<typename T> inline T exclude(T const &x, T const &a, T const &b)
{
	if ((x < a) || (b < x))
		return x;

	if ((b - x) > (x - a))
		return a;

	return b;
}

///
/// 
/// 

template<typename T> inline T deg2rad(T theta)
{
	return LUCID_CORE_NUMBERS::pi<T> * theta / T(180);
}

template<typename T> inline T rad2deg(T theta)
{
	return T(180) * theta / LUCID_CORE_NUMBERS::pi<T>;
}

///
///	power "wrapper"
///

template<typename T> inline T pow(T x, T y)
{
	return T(std::pow(x, y));
}

template<typename T> inline T exp(T x)
{
	return T(std::exp(x));
}

///
///	square root "wrapper"
///

template<typename T> inline T sqrt(T x)
{
	return T(std::sqrt(x));
}

///
///	cosine "wrappers"
///

template<typename T> inline T cos(T theta)
{
	return T(std::cos(theta));
}

template<typename T> inline T acos(T x)
{
	return T(std::acos(x));
}

///
///	sine "wrappers"
///

template<typename T> inline T sin(T theta)
{
	return T(std::sin(theta));
}

template<typename T> inline T asin(T x)
{
	return T(std::asin(x));
}

///
///	tangent "wrappers"
///

template<typename T> inline T tan(T theta)
{
	return T(std::tan(theta));
}

template<typename T> inline T atan(T x)
{
	return T(std::atan(x));
}

template<typename T> inline T atan2(T y, T x)
{
	return T(std::atan2(y, x));
}

///	quaternion to matrix
///
///
template<typename T> inline LUCID_MATRIX(T, 3, 3) matrixFromQuaternion(LUCID_QUATERNION(T) const &q) 
{
	T xx = q.x * q.x;
	T xy = q.x * q.y;
	T xz = q.x * q.z;
	T xw = q.x * q.w;
	T yy = q.y * q.y;
	T yz = q.y * q.z;
	T yw = q.y * q.w;
	T zz = q.z * q.z;
	T zw = q.z * q.w;

	return LUCID_MATRIX(T, 3, 3)(
		T(1) - T(2) * ( yy + zz ),        T(2) * ( xy - zw ),        T(2) * ( xz + yw ),
			   T(2) * ( xy + zw ), T(1) - T(2) * ( xx + zz ),        T(2) * ( yz - xw ),
			   T(2) * ( xz - yw ),        T(2) * ( yz + xw ), T(1) - T(2) * ( xx + yy )
	);
}

///	matrix to quaternion
///
///
template<typename T> inline LUCID_QUATERNION(T) quaternionFromMatrix(LUCID_MATRIX(T, 3, 3) const &R)
{
	LUCID_QUATERNION(T) q;

	q.w = T(0.5) * sqrt(T(1) + R.xx + R.yy + R.zz);
	T coeff = T(1) / (T(4) * q.w);

	q.x = coeff * (R.zy - R.yz);
	q.y = coeff * (R.xz - R.zx);
	q.z = coeff * (R.yx - R.xy);

	return q;
}

///	intersects
///
///	Segment intersects Plane
/// 0 <= t <= 1 segment intersects plane (point = (p1 - p0) * t + p0)
template<typename T> inline T intersects(LUCID_PLANE(T, 3) const &plane, LUCID_VECTOR(T, 3) const &p0, LUCID_VECTOR(T, 3) const &p1)
{
	T D = LUCID_MATH::dot(p1 - p0, plane.n);
	if ((D * D) > LUCID_CORE_NUMBERS::tolsq<T>)
		return (plane.d - LUCID_MATH::dot(plane.n, p0)) / D;
	return LUCID_CORE_NUMBERS::inf<T>;
}

///	intersects
///
///	Sphere intersects AABB
template<typename T> inline bool intersects(LUCID_SPHERE(T) const &sphere, LUCID_BOX(T) const &aabb)
{
	T RR = sphere.radius * sphere.radius;

	auto S = [](T x) { return x * x; };

	if (sphere.center.x < aabb.min.x)
	{
		RR -= S(sphere.center.x - aabb.min.x);
	}
	else if (sphere.center.x > aabb.max.x)
	{
		RR -= S(sphere.center.x - aabb.max.x);
	}

	if (sphere.center.y < aabb.min.y)
	{
		RR -= S(sphere.center.y - aabb.min.y);
	}
	else if (sphere.center.y > aabb.max.y)
	{
		RR -= S(sphere.center.y - aabb.max.y);
	}

	if (sphere.center.z < aabb.min.z)
	{
		RR -= S(sphere.center.z - aabb.min.z);
	}
	else if (sphere.center.z > aabb.max.z)
	{
		RR -= S(sphere.center.z - aabb.max.z);
	}

	return RR > T(0);
}

///	intersects
///
///	Frustum intersects AABB
template<typename T> inline bool intersects(LUCID_FRUSTUM(T) const &frustum, LUCID_BOX(T) const &aabb)
{
	int const ELEMENT_COUNT = 3;
	int const CORNER_COUNT = Frustum<T>::CORNER_COUNT;

	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		int left = 0;
		int right = 0;

		for (int j = 0; j < CORNER_COUNT; ++j)
		{
			left = (frustum.corners[j].elements[i] < aabb.min[i]) ? left + 1 : left;
			right = (aabb.max[i] < frustum.corners[j].elements[i]) ? right + 1 : right;
		}

		if ((CORNER_COUNT == left) || (CORNER_COUNT == right))
		{
			return false;
		}
	}

	LUCID_VECTOR(T, 3) const boxCorners[] =
	{
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.min.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.min.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.max.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.max.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.min.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.min.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.max.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.max.y, aabb.max.z),
	};

	for (int i = 0; i < Frustum<T>::PLANE_COUNT; ++i)
	{
		LUCID_PLANE(T, 3) const &plane = frustum.planes[i];

		int behind = 0;
		for (int j = 0; j < CORNER_COUNT; ++j)
		{
			if (dot(plane, boxCorners[j]) > 0.f)
				break;
			++behind;
		}

		if (CORNER_COUNT == behind)
		{
			return false;
		}
	}

	return true;
}

///	project
///
///	project a specified number of 3D points onto an axis and return a min/max range.
template<typename T> inline std::pair<T, T> project(LUCID_VECTOR(T, 3) const *vertices, size_t vertexCount, LUCID_VECTOR(T, 3) const &axis)
{
	std::pair<T, T> result(0, 0);

	result.first = dot(vertices[0], axis);
	result.second = result.first;

	for (int32_t i = 1; i < vertexCount; ++i)
	{
		T D = dot(vertices[i], axis);

		result.first = std::min(result.first, D);
		result.second = std::max(result.second, D);
	}

	return result;
}

///	intersect
///
///	3D AABB intersects 3D triangle
template<typename T> inline bool intersect(LUCID_BOX(T) const &box, LUCID_VECTOR(T, 3) const &v_i, LUCID_VECTOR(T, 3) const &v_j, LUCID_VECTOR(T, 3) const &v_k)
{
	LUCID_VECTOR(T, 3) const boxNormals[] =
	{
		LUCID_VECTOR(T, 3)(1, 0, 0),
		LUCID_VECTOR(T, 3)(0, 1, 0),
		LUCID_VECTOR(T, 3)(0, 0, 1),
	};

	LUCID_VECTOR(T, 3) const faceVertices[] = { v_i, v_j, v_k, };

	for (size_t i = 0; i < 3; ++i)
	{
		std::pair<T, T> range = project(faceVertices, 3, boxNormals[i]);
		if ((range.second < box.min[i]) || (box.max[i] < range.first))
		{
			return false;
		}
	}

	LUCID_VECTOR(T, 3) const boxVertices[] =
	{
		LUCID_VECTOR(T, 3)(box.min.x, box.min.y, box.min.z),
		LUCID_VECTOR(T, 3)(box.min.x, box.min.y, box.max.z),
		LUCID_VECTOR(T, 3)(box.min.x, box.max.y, box.min.z),
		LUCID_VECTOR(T, 3)(box.min.x, box.max.y, box.max.z),
		LUCID_VECTOR(T, 3)(box.max.x, box.min.y, box.min.z),
		LUCID_VECTOR(T, 3)(box.max.x, box.min.y, box.max.z),
		LUCID_VECTOR(T, 3)(box.max.x, box.max.y, box.min.z),
		LUCID_VECTOR(T, 3)(box.max.x, box.max.y, box.max.z),
	};

	{
		LUCID_VECTOR(T, 3) const faceNormal = normalize(cross(v_j - v_i, v_k - v_i));
		T D = dot(faceNormal, v_i);

		std::pair<T, T> range = project(boxVertices, 8, faceNormal);
		if ((range.second < D) || (D < range.first))
		{
			return false;
		}
	}

	LUCID_VECTOR(T, 3) const faceEdges[] = { v_j - v_i, v_k - v_j, v_i - v_k, };

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			LUCID_VECTOR(T, 3) axis = cross(faceEdges[i], boxNormals[j]);

			std::pair<T, T> boxRange = project(boxVertices, 8, axis);
			std::pair<T, T> faceRange = project(faceVertices, 3, axis);
			if ((boxRange.second < faceRange.first) || (faceRange.second < boxRange.first))
			{
				return false;
			}
		}
	}

	return true;
}

///	contains
///
///	3D Frustum contains 3D AABB
template<typename T> inline bool contains(LUCID_FRUSTUM(T) const &frustum, LUCID_BOX(T) const &aabb)
{
	int const CORNER_COUNT = 8;

	LUCID_VECTOR(T, 3) const corners[] =
	{
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.min.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.min.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.max.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.min.x, aabb.max.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.min.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.min.y, aabb.max.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.max.y, aabb.min.z),
		LUCID_VECTOR(T, 3)(aabb.max.x, aabb.max.y, aabb.max.z),
	};

	for (size_t i = 0; i < LUCID_FRUSTUM(T)::PLANE_COUNT; ++i)
	{
		LUCID_PLANE(T, 3) const &plane = frustum.planes[i];

		for (size_t j = 0; j < CORNER_COUNT; ++j)
		{
			if (dot(plane, corners[j]) < T(0))
				return false;
		}
	}

	return true;
}

///	areaProjected
///
///
template<typename T> inline T areaProjected(LUCID_MATRIX(T, 4, 4) const &viewProjMatrix, T const &znear, LUCID_BOX(T) const &aabb)
{
	size_t const CORNER_COUNT = 8;

	LUCID_VECTOR(T, 4) const corners[] =
	{
		LUCID_VECTOR(T, 4)(aabb.min.x, aabb.min.y, aabb.min.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.min.x, aabb.min.y, aabb.max.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.min.x, aabb.max.y, aabb.min.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.min.x, aabb.max.y, aabb.max.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.max.x, aabb.min.y, aabb.min.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.max.x, aabb.min.y, aabb.max.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.max.x, aabb.max.y, aabb.min.z, T(1)),
		LUCID_VECTOR(T, 4)(aabb.max.x, aabb.max.y, aabb.max.z, T(1)),
	};

	LUCID_VECTOR(T, 4) projected = viewProjMatrix * corners[0];
	projected = projected / exclude(T(projected.w), -znear, znear);

	T xmin = projected.x;
	T ymin = projected.y;

	T xmax = projected.x;
	T ymax = projected.y;

	for (size_t i = 1; i < CORNER_COUNT; ++i)
	{
		projected = viewProjMatrix * corners[i];
		projected = projected / exclude(T(projected.w), -znear, znear);

		xmin = std::min(xmin, projected.x);
		ymin = std::min(ymin, projected.y);

		xmax = std::max(xmax, projected.x);
		ymax = std::max(ymax, projected.y);
	}

	return (xmax - xmin) * (ymax - ymin);
}

LUCID_MATH_END

