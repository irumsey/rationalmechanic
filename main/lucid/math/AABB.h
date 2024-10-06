#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Sphere.h>

#ifdef min
#	undef min
#	undef max
#endif

LUCID_MATH_BEGIN

///	AABB
///
///	Axis aligned bounding box
/// 
/// Note: min/max default to whatever the specified type T defaults to.
template<typename T, size_t DIM> struct AABB
{
	Vector<T, DIM> min;
	Vector<T, DIM> max;

	AABB() = default;

	AABB(Vector<T, DIM> const &min, Vector<T, DIM> const &max)
		: min(min)
		, max(max)
	{
	}

	~AABB() = default;

	Vector<T, DIM> center() const
	{
		return T(0.5) * (max + min);
	}

	Vector<T, DIM> extent() const
	{
		return T(0.5) * (max - min);
	}
};

///
///
///
template<typename T> struct AABB<T, 1>
{
	T min = T();
	T max = T();

	AABB() = default;

	AABB(T const &min, T const &max)
		: min(min)
		, max(max)
	{
	}

	~AABB() = default;

	T center() const
	{
		return T(0.5) * (max + min);
	}

	T extent() const
	{
		return T(0.5) * (max - min);
	}
};

///	fit
///
///	expand the given box to include the specified point.
template<typename T, size_t DIM> inline AABB<T, DIM> &fit(AABB<T, DIM> &box, Vector<T, DIM> const &point)
{
	for (size_t i = 0; i < DIM; ++i)
	{
		box.min[i] = std::min(box.min[i], point[i]);
		box.max[i] = std::max(box.max[i], point[i]);
	}
	return box;
}

///	fit
///
///	convienience method for creating a box which fits a triangle.
template<typename T, size_t DIM> inline AABB<T, DIM> fit(Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
{
	AABB<T, DIM> box;

	box.min = box.max = v_i;
	fit(box, v_j);
	fit(box, v_k);

	return box;
}

///	fit
///
///
template<typename T, size_t DIM> inline AABB<T, DIM> &fit(AABB<T, DIM> &lval, AABB<T, DIM> const &rhs)
{
	for (size_t i = 0; i < DIM; ++i)
	{
		lval.min[i] = std::min(lval.min[i], rhs.min[i]);
		lval.max[i] = std::max(lval.max[i], rhs.max[i]);
	}
	return lval;
}

///	contains
///
///
template<typename T, size_t DIM> inline bool contains(AABB<T, DIM> const &box, Vector<T, DIM> const &point)
{
	for (size_t i = 0; i < DIM; ++i)
	{
		if ((point[i] < box.min[i]) || (box.max[i] < point[i]))
			return false;
	}
	return true;
}

///
///
///
template<typename T, size_t DIM> inline bool contains(AABB<T, DIM> const &box, Sphere<T, DIM> const &sphere)
{
	for (size_t i = 0; i < DIM; ++i)
	{
		if ((sphere.center[i] - sphere.radius) < box.min[i])
			return false;

		if ((sphere.center[i] + sphere.radius) > box.max[i])
			return false;
	}
	return true;
}

///
///
///
template<typename T, size_t DIM> inline bool looselyContains(AABB<T, DIM> const &box, Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
{
	AABB<T, DIM> faceBox = fit(v_i, v_j, v_k);
	Vector<T, DIM> faceCenter = faceBox.center();

	if (!contains(box, faceCenter))
	{
		return false;
	}

	Vector<T, DIM> faceExtent = faceBox.extent();
	Vector<T, DIM> boxExtent = box.extent();

	for (size_t i = 0; i < DIM; ++i)
	{
		if (faceExtent[i] > boxExtent[i])
			return false;
	}

	return true;
}

LUCID_MATH_END