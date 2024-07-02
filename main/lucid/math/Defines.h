#pragma once

#include <lucid/core/Defines.h>
#include <lucid/units/Defines.h>

#define LUCID_MATH ::lucid::math

#define LUCID_MATH_BEGIN LUCID_BEGIN namespace math {
#define LUCID_MATH_END } LUCID_END

#define LUCID_SCALAR(T, S, Q) LUCID_MATH::Scalar<T, LUCID_UNITS::Unit<S, Q> >

#define LUCID_VECTOR(T, DIM, S, Q) LUCID_MATH::Vector<T, DIM, LUCID_UNITS::Unit<S, Q> >

#define LUCID_MATRIX(T, ROWS, COLS, S, Q) LUCID_MATH::Matrix<T, ROWS, COLS, LUCID_UNITS::Unit<S, Q> >

#define LUCID_QUATERNION(T) LUCID_MATH::Quaternion<T>

#define LUCID_SPHERE(T, S, Q) LUCID_MATH::Sphere<T, 3, LUCID_UNITS::Unit<S, Q> >

#define LUCID_PLANE(T, DIM, S, Q) LUCID_MATH::Plane<T, DIM, S, Q>

#define LUCID_FRUSTUM(T, S, Q) LUCID_MATH::Frustum<T, S, Q>

#define LUCID_BOX(T, S, Q) LUCID_MATH::AABB<T, 3, LUCID_UNITS::Unit<S, Q> >