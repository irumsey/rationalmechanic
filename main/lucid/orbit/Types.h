#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Defines.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>
#include <lucid/orbit/Defines.h>

LUCID_ORBIT_BEGIN

///
/// TBD: for initial porting purposes...
/// 
///	first, use a dimensionless quantity...
/// 
/// then, when that works...
/// 
/// second, build up use of actual dimensions (time, length, mass, etc...)
/// 

typedef float64_t scalar_t;

typedef LUCID_MATH::Vector<float64_t, 2>  vector2_t;
typedef LUCID_MATH::Vector<float64_t, 3>  vector3_t;
typedef LUCID_MATH::Vector<float64_t, 4>  vector4_t;

typedef LUCID_MATH::Matrix<float64_t, 3, 3> matrix3x3_t;
typedef LUCID_MATH::Matrix<float64_t, 4, 4> matrix4x4_t;

typedef LUCID_MATH::AABB<float64_t, 3> aabb3_t;

typedef LUCID_MATH::Frustum<float64_t> frustum_t;

LUCID_ORBIT_END