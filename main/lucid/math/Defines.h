#pragma once

#include <lucid/core/Defines.h>
#include <lucid/units/Defines.h>

#define LUCID_MATH ::lucid::math

#define LUCID_MATH_BEGIN LUCID_BEGIN namespace math {
#define LUCID_MATH_END } LUCID_END

#define LUCID_SCALAR(T,S,Q) LUCID_MATH::Scalar<T,LUCID_UNITS::Unit<S,Q> >

#define LUCID_VECTOR(T, DIM) LUCID_MATH::Vector<T, DIM>

#define LUCID_MATRIX(T, ROWS, COLS) LUCID_MATH::Matrix<T, ROWS, COLS>

#define LUCID_QUATERNION(T) LUCID_MATH::Quaternion<T>

#define LUCID_SPHERE(T) LUCID_MATH::Sphere<T, 3>

#define LUCID_PLANE(T, DIM) LUCID_MATH::Plane<T, DIM>

#define LUCID_FRUSTUM(T) LUCID_MATH::Frustum<T>

#define LUCID_BOX(T) LUCID_MATH::AABB<T, 3>