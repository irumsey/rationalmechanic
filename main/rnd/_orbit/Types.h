#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Math.h>
#include <rnd/_orbit/Defines.h>

ORBIT_BEGIN

typedef float64_t scalar_t;

typedef LUCID_MATH::Vector<scalar_t, 2> vector2_t;
typedef LUCID_MATH::Vector<scalar_t, 3> vector3_t;
typedef LUCID_MATH::Vector<scalar_t, 4> vector4_t;

typedef LUCID_MATH::Matrix<scalar_t, 3, 3> matrix3x3_t;
typedef LUCID_MATH::Matrix<scalar_t, 4, 4> matrix4x4_t;

typedef LUCID_MATH::Quaternion<scalar_t> quaternion_t;

typedef LUCID_MATH::AABB<scalar_t, 3> aabb3_t;

typedef LUCID_MATH::Frustum<scalar_t> frustum_t;

struct Date
{
	int32_t  year = 1970;
	int32_t month =    1;
	int32_t   day =    1;
};

struct Time
{
	int32_t    hour = 0;
	int32_t  minute = 0;
	scalar_t second = 0.0;
};

ORBIT_END