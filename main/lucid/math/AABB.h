#pragma once

#include <lucid/math/Vector.h>
#include <lucid/math/Sphere.h>
#include <lucid/math/Constants.h>

#ifdef min
#	undef min
#	undef max
#endif

namespace lucid {
namespace math {

	///
	///
	///
	template<typename T, int DIM> struct AABB
	{
		Vector<T, DIM> min;
		Vector<T, DIM> max;

		AABB() = default;

		AABB(Vector<T, DIM> const &min, Vector<T, DIM> const &max)
			: min(min)
			, max(max)
		{
		}

		Vector<T, DIM> center() const
		{
			return constants::half<T>() * (max + min);
		}

		Vector<T, DIM> extent() const
		{
			return constants::half<T>() * (max - min);
		}
	};

	///
	///
	///
	template<typename T> struct AABB<T,1>
	{
		T min = constants::neg_inf<T>();
		T max = constants::pos_inf<T>();

		AABB() = default;

		AABB(T const &min, T const &max)
			: min(min)
			, max(max)
		{
		}

		T center() const
		{
			return constants::half<T>() * (max + min);
		}

		T extent() const
		{
			return constants::half<T>() * (max - min);
		}
	};

	///
	///
	///
	template<typename T> struct AABB<T,2>
	{
		Vector<T, 2> min = Vector<T,2>(constants::neg_inf<T>(), constants::neg_inf<T>());
		Vector<T, 2> max = Vector<T,2>(constants::pos_inf<T>(), constants::pos_inf<T>());

		AABB() = default;

		AABB(Vector<T, 2> const &min, Vector<T, 2> const &max)
			: min(min)
			, max(max)
		{
		}

		Vector<T, 2> center() const
		{
			return constants::half<T>() * (max + min);
		}

		Vector<T, 2> extent() const
		{
			return constants::half<T>() * (max - min);
		}
	};

	///
	///
	///
	template<typename T> struct AABB<T,3>
	{
		Vector<T, 3> min = Vector<T,3>(constants::neg_inf<T>(), constants::neg_inf<T>(), constants::neg_inf<T>());
		Vector<T, 3> max = Vector<T,3>(constants::pos_inf<T>(), constants::pos_inf<T>(), constants::pos_inf<T>());

		AABB() = default;

		AABB(Vector<T, 3> const &min, Vector<T, 3> const &max)
			: min(min)
			, max(max)
		{
		}

		Vector<T, 3> center() const
		{
			return constants::half<T>() * (max + min);
		}

		Vector<T, 3> extent() const
		{
			return constants::half<T>() * (max - min);
		}
	};

	///	fit
	///
	///	expand the given box to include the specified point.
	template<typename T, int DIM> inline void fit(AABB<T, DIM> &box, Vector<T, DIM> const &point)
	{
		for (int i = 0; i < DIM; ++i)
		{
			box.min[i] = std::min(box.min[i], point[i]);
			box.max[i] = std::max(box.max[i], point[i]);
		}
	}

	///	fit
	///
	///	convienience method for creating a box which fits a triangle.
	template<typename T, int DIM> inline AABB<T, DIM> fit(Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
	{
		AABB<T, DIM> box;

		fit(box, v_i);
		fit(box, v_j);
		fit(box, v_k);

		return box;
	}

	///	contains
	///
	///
	template<typename T, int DIM> inline bool contains(AABB<T, DIM> const &box, Vector<T, DIM> const &point)
	{
		for (int i = 0; i < DIM; ++i)
		{
			if ((point[i] < box.min[i]) || (box.max[i] < point[i]))
				return false;
		}
		return true;
	}

	///
	///
	///
	template<typename T, int DIM> inline bool contains(AABB<T, DIM> const &box, Sphere<T, DIM> const &sphere)
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
	template<typename T, int DIM> inline bool looselyContains(AABB<T, DIM> const &box, Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
	{
		AABB<T, DIM> faceBox = fit(v_i, v_j, v_k);
		Vector<T, DIM> faceCenter = faceBox.center();

		if (!contains(box, faceCenter))
		{
			return false;
		}

		Vector<T, DIM> faceExtent = faceBox.extent();
		Vector<T, DIM> boxExtent = box.extent();

		for (int i = 0; i < DIM; ++i)
		{
			if (faceExtent[i] > boxExtent[i])
				return false;
		}

		return true;
	}


}	///	math
}	///	lucid
