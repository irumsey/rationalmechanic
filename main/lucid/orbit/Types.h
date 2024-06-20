#pragma once

#include <lucid/core/Types.h>
#include <lucid/units/System.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/math/AABB.h>
#include <lucid/math/Frustum.h>

namespace lucid {
namespace orbit {
	
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
	typedef units::quantity::pure quantity_none;
	typedef units::Unit<system_of_measure, quantity_none> unit_of_measure;

	typedef math::Scalar<float64_t, unit_of_measure> scalar_t;

	typedef math::Vector<float64_t, 2, unit_of_measure>  vector2_t;
	typedef math::Vector<float64_t, 3, unit_of_measure>  vector3_t;
	typedef math::AABB<float64_t, 3, unit_of_measure> aabb3_t;

	typedef math::Matrix<float64_t, 3, 3, unit_of_measure> matrix3x3_t;

	typedef math::Frustum <float64_t, system_of_measure, quantity_none> frustum_t;

}	///	orbit
}	///	lucid
