#pragma once

#include <string>
#include <lucid/core/Constants.h>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace math {
namespace constants {

		LUCID_CONSTANT(      zero, 0.0)
		LUCID_CONSTANT(       one, 1.0)
		LUCID_CONSTANT(      half, 0.5)

		LUCID_CONSTANT(    two_pi, 6.283185307179586476925286766559)
		LUCID_CONSTANT(        pi, 3.141592653589793238462643383279)
		LUCID_CONSTANT(   half_pi, 1.570796326794896619231321691639)
		LUCID_CONSTANT(quarter_pi, 0.785398163397448309615660845819)

		LUCID_CONSTANT(         e, 2.718281828459045235360287471352)

		LUCID_CONSTANT(   pos_inf,  1e38)
		LUCID_CONSTANT(   neg_inf, -1e38)

		LUCID_CONSTANT(       tol, 0.0000001)
		LUCID_CONSTANT(   tol_tol, 0.00000000000001)

}	///	constants
}	///	math
}	///	lucid
