#pragma once

#include <algorithm>
#include <lucid/math/Constants.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/Quaternion.h>
#include <lucid/math/Sphere.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>

#ifdef min
#	undef min
#	undef max
#endif

namespace lucid {
namespace math {

	///	quaternion to matrix
	///
	///
	template<typename T> Matrix<T,3,3> matrixFromQuaternion(Quaternion<T> const &q) 
	{
		float xx = q.x * q.x;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float xw = q.x * q.w;
		float yy = q.y * q.y;
		float yz = q.y * q.z;
		float yw = q.y * q.w;
		float zz = q.z * q.z;
		float zw = q.z * q.w;

		return Matrix<T,3,3>(
			1 - 2 * ( yy + zz ),     2 * ( xy - zw ),     2 * ( xz + yw ), 0,
			    2 * ( xy + zw ), 1 - 2 * ( xx + zz ),     2 * ( yz - xw ), 0,
			    2 * ( xz - yw ),     2 * ( yz + xw ), 1 - 2 * ( xx + yy ), 0,
			                  0,                   0,                   0, 1
		);
	}

	///	matrix to quaternion
	///
	///
	template<typename T> Quaternion<T> quaternionFromMatrix(Matrix<T,3,3> const &R)
	{
		Quaternion<T> q;

		q.w = 0.5f * sqrt(1.f + R.xx + R.yy + R.zz);
		T coeff = 1.f / (4.f * q.w);

		q.x = coeff * (R.zy - R.yz);
		q.y = coeff * (R.xz - R.zx);
		q.z = coeff * (R.yx - R.xy);

		return q;
	}

	///	intersects
	///
	///	Sphere intersects AABB
	template<typename T> inline bool intersects(Sphere<T, 3> const &sphere, AABB<T, 3> const &aabb)
	{
		float32_t RR = sphere.radius * sphere.radius;

		auto S = [](float x) { return x * x; };

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

		return RR > constants::zero<T>();
	}

	///	intersects
	///
	///	Frustum intersects AABB
	template<typename T> inline bool intersects(Frustum<T> const &frustum, AABB<T, 3> const &aabb)
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

		Vector<T, 3> const boxCorners[] =
		{
			Vector<T, 3>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector<T, 3>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector<T, 3>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector<T, 3>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector<T, 3>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector<T, 3>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector<T, 3>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector<T, 3>(aabb.max.x, aabb.max.y, aabb.max.z),
		};

		for (int i = 0; i < Frustum<T>::PLANE_COUNT; ++i)
		{
			Plane<T, 3> const &plane = frustum.planes[i];

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
	template<typename T> inline std::pair<T, T> project(Vector<T, 3> const *vertices, int32_t vertexCount, Vector<T, 3> const &axis)
	{
		std::pair<T, T> result(0, 0);

		result.first = dot(vertices[0], axis);
		result.second = result.first;

		for (int32_t i = 1; i < vertexCount; ++i)
		{
			float32_t D = dot(vertices[i], axis);

			result.first = std::min(result.first, D);
			result.second = std::max(result.second, D);
		}

		return result;
	}

	///	intersect
	///
	///	3D AABB intersects 3D triangle
	template<typename T> inline bool intersect(AABB<T, 3> const &box, Vector<T, 3> const &v_i, Vector<T, 3> const &v_j, Vector<T, 3> const &v_k)
	{
		Vector<T, 3> const boxNormals[] =
		{
			Vector<T, 3>(1, 0, 0),
			Vector<T, 3>(0, 1, 0),
			Vector<T, 3>(0, 0, 1),
		};

		Vector<T, 3> const faceVertices[] = { v_i, v_j, v_k, };

		for (int i = 0; i < 3; ++i)
		{
			std::pair<T, T> range = project(faceVertices, 3, boxNormals[i]);
			if ((range.second < box.min[i]) || (box.max[i] < range.first))
			{
				return false;
			}
		}

		Vector<T, 3> const boxVertices[] =
		{
			Vector<T, 3>(box.min.x, box.min.y, box.min.z),
			Vector<T, 3>(box.min.x, box.min.y, box.max.z),
			Vector<T, 3>(box.min.x, box.max.y, box.min.z),
			Vector<T, 3>(box.min.x, box.max.y, box.max.z),
			Vector<T, 3>(box.max.x, box.min.y, box.min.z),
			Vector<T, 3>(box.max.x, box.min.y, box.max.z),
			Vector<T, 3>(box.max.x, box.max.y, box.min.z),
			Vector<T, 3>(box.max.x, box.max.y, box.max.z),
		};

		{
			Vector<T, 3> const faceNormal = normalize(cross(v_j - v_i, v_k - v_i));
			float32_t D = dot(faceNormal, v_i);

			std::pair<T, T> range = project(boxVertices, 8, faceNormal);
			if ((range.second < D) || (D < range.first))
			{
				return false;
			}
		}

		Vector<T, 3> const faceEdges[] = { v_j - v_i, v_k - v_j, v_i - v_k, };

		for (int32_t i = 0; i < 3; ++i)
		{
			for (int32_t j = 0; j < 3; ++j)
			{
				Vector<T, 3> axis = cross(faceEdges[i], boxNormals[j]);

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
	template<typename T> inline bool contains(Frustum<T> const &frustum, AABB<T, 3> const &aabb)
	{
		int const CORNER_COUNT = 8;

		Vector<T, 3> const corners[] =
		{
			Vector<T, 3>(aabb.min.x, aabb.min.y, aabb.min.z),
			Vector<T, 3>(aabb.min.x, aabb.min.y, aabb.max.z),
			Vector<T, 3>(aabb.min.x, aabb.max.y, aabb.min.z),
			Vector<T, 3>(aabb.min.x, aabb.max.y, aabb.max.z),
			Vector<T, 3>(aabb.max.x, aabb.min.y, aabb.min.z),
			Vector<T, 3>(aabb.max.x, aabb.min.y, aabb.max.z),
			Vector<T, 3>(aabb.max.x, aabb.max.y, aabb.min.z),
			Vector<T, 3>(aabb.max.x, aabb.max.y, aabb.max.z),
		};

		for (int i = 0; i < Frustum<T>::PLANE_COUNT; ++i)
		{
			Plane<T, 3> const &plane = frustum.planes[i];

			for (int j = 0; j < CORNER_COUNT; ++j)
			{
				if (dot(plane, corners[j]) < 0.f)
					return false;
			}
		}

		return true;
	}

	///	areaProjected
	///
	///
	template<typename T> inline T areaProjected(Matrix<T, 4, 4> const &viewProjMatrix, T const &znear, AABB<T, 3> const &aabb)
	{
		int const CORNER_COUNT = 8;

		Vector<T, 4> const corners[] =
		{
			Vector<T, 4>(aabb.min.x, aabb.min.y, aabb.min.z, constants::one<T>()),
			Vector<T, 4>(aabb.min.x, aabb.min.y, aabb.max.z, constants::one<T>()),
			Vector<T, 4>(aabb.min.x, aabb.max.y, aabb.min.z, constants::one<T>()),
			Vector<T, 4>(aabb.min.x, aabb.max.y, aabb.max.z, constants::one<T>()),
			Vector<T, 4>(aabb.max.x, aabb.min.y, aabb.min.z, constants::one<T>()),
			Vector<T, 4>(aabb.max.x, aabb.min.y, aabb.max.z, constants::one<T>()),
			Vector<T, 4>(aabb.max.x, aabb.max.y, aabb.min.z, constants::one<T>()),
			Vector<T, 4>(aabb.max.x, aabb.max.y, aabb.max.z, constants::one<T>()),
		};

		Vector<T, 4> projected = viewProjMatrix * corners[0];
		projected = projected / exclude(projected.w, -znear, znear);

		T xmin = projected.x;
		T ymin = projected.y;

		T xmax = projected.x;
		T ymax = projected.y;

		for (int i = 1; i < CORNER_COUNT; ++i)
		{
			projected = viewProjMatrix * corners[i];
			projected = projected / exclude(projected.w, -znear, znear);

			xmin = std::min(xmin, projected.x);
			ymin = std::min(ymin, projected.y);

			xmax = std::max(xmax, projected.x);
			ymax = std::max(ymax, projected.y);
		}

		return (xmax - xmin) * (ymax - ymin);
	}




}	///	math
}	///	lucid
