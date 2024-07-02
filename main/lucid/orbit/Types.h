#pragma once

#include <lucid/core/Types.h>
#include <lucid/units/Defines.h>
#include <lucid/units/System.h>
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

struct system_of_measure {};
typedef LUCID_UNITS::quantity::pure quantity_none;
typedef LUCID_UNITS::Unit<system_of_measure, quantity_none> unit_of_measure;

typedef LUCID_MATH::Scalar<float64_t, unit_of_measure> scalar_t;

typedef LUCID_MATH::Vector<float64_t, 2, unit_of_measure>  vector2_t;
typedef LUCID_MATH::Vector<float64_t, 3, unit_of_measure>  vector3_t;
typedef LUCID_MATH::Vector<float64_t, 4, unit_of_measure>  vector4_t;

typedef LUCID_MATH::Matrix<float64_t, 3, 3, unit_of_measure> matrix3x3_t;
typedef LUCID_MATH::Matrix<float64_t, 4, 4, unit_of_measure> matrix4x4_t;

typedef LUCID_MATH::AABB<float64_t, 3, unit_of_measure> aabb3_t;

typedef LUCID_MATH::Frustum<float64_t, system_of_measure, quantity_none> frustum_t;

LUCID_ORBIT_END