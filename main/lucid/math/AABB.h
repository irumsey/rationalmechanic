#pragma once

#include <lucid/math/Vector.h>
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
	template<class T, int DIM> struct AABB
	{
		Vector<T, DIM> min = Vector<T, DIM>(constants::pos_inf<T>(), constants::pos_inf<T>(), constants::pos_inf<T>());
		Vector<T, DIM> max = Vector<T, DIM>(constants::neg_inf<T>(), constants::neg_inf<T>(), constants::neg_inf<T>());

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

	///	fit
	///
	///	expand the given box to include the specified point.
	template<class T, int DIM> inline void fit(AABB<T, DIM> &box, Vector<T, DIM> const &point)
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
	template<class T, int DIM> inline AABB<T, DIM> fit(Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
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
	template<class T, int DIM> inline bool contains(AABB<T, DIM> const &box, Vector<T, DIM> const &point)
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
	template<class T, int DIM> inline bool looselyContains(AABB<T, DIM> const &box, Vector<T, DIM> const &v_i, Vector<T, DIM> const &v_j, Vector<T, DIM> const &v_k)
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
