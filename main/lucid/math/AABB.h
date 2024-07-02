#pragma once

#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Sphere.h>

#ifdef min
#	undef min
#	undef max
#endif

LUCID_MATH_BEGIN

///
///
///
template<typename T, size_t DIM, typename U> struct AABB
{
	Vector<T, DIM, U> min;
	Vector<T, DIM, U> max;

	AABB() = default;

	AABB(Vector<T, DIM, U> const &min, Vector<T, DIM, U> const &max)
		: min(min)
		, max(max)
	{
	}

	~AABB() = default;

	Vector<T, DIM, U> center() const
	{
		return Scalar<T, U>(0.5) * (max + min);
	}

	Vector<T, DIM, U> extent() const
	{
		return Scalar<T, U>(0.5) * (max - min);
	}
};

///
///
///
template<typename T, typename U> struct AABB<T, 1, U>
{
	Scalar<T, U> min;
	Scalar<T, U> max;

	AABB() = default;

	AABB(T const &min, T const &max)
		: min(min)
		, max(max)
	{
	}

	~AABB() = default;

	Scalar<T, U> center() const
	{
		return Scalar<T, U>(0.5) * (max + min);
	}

	Scalar<T, U> extent() const
	{
		return Scalar<T, U>(0.5) * (max - min);
	}
};

///	fit
///
///	expand the given box to include the specified point.
template<typename T, size_t DIM, typename U>
inline AABB<T, DIM, U> &fit(AABB<T, DIM, U> &box, Vector<T, DIM, U> const &point)
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
template<typename T, size_t DIM, typename U>
inline AABB<T, DIM, U> fit(Vector<T, DIM, U> const &v_i, Vector<T, DIM, U> const &v_j, Vector<T, DIM, U> const &v_k)
{
	AABB<T, DIM> box;

	fit(box, v_i);
	fit(box, v_j);
	fit(box, v_k);

	return box;
}

///	fit
///
///
template<typename T, size_t DIM, typename U>
inline AABB<T, DIM, U> &fit(AABB<T, DIM, U> &lval, AABB<T, DIM, U> const &rhs)
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
template<typename T, size_t DIM, typename U>
inline bool contains(AABB<T, DIM, U> const &box, Vector<T, DIM, U> const &point)
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
template<typename T, size_t DIM, typename U>
inline bool contains(AABB<T, DIM, U> const &box, Sphere<T, DIM, U> const &sphere)
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
template<typename T, size_t DIM, typename U>
inline bool looselyContains(AABB<T, DIM, U> const &box, Vector<T, DIM, U> const &v_i, Vector<T, DIM, U> const &v_j, Vector<T, DIM, U> const &v_k)
{
	AABB<T, DIM, U> faceBox = fit(v_i, v_j, v_k);
	Vector<T, DIM, U> faceCenter = faceBox.center();

	if (!contains(box, faceCenter))
	{
		return false;
	}

	Vector<T, DIM, U> faceExtent = faceBox.extent();
	Vector<T, DIM, U> boxExtent = box.extent();

	for (size_t i = 0; i < DIM; ++i)
	{
		if (faceExtent[i] > boxExtent[i])
			return false;
	}

	return true;
}

LUCID_MATH_END