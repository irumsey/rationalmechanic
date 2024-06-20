#pragma once

#include <lucid/units/System.h>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>

#pragma push_macro("QUANTITY_PURE")
#define QUANTITY_PURE ::lucid::units::quantity::pure

#pragma push_macro("SCALAR")
#define SCALAR(T, S, Q) ::lucid::math::Scalar<T, ::lucid::units::Unit<S, Q> >

#pragma push_macro("VECTOR")
#define VECTOR(T, DIM, S, Q) ::lucid::math::Vector<T, DIM, ::lucid::units::Unit<S, Q> >

#pragma push_macro("PLANE")
#define PLANE(T, DIM, S, Q) ::lucid::math::Plane<T, DIM, S, Q>

namespace lucid {
namespace math {

	///	Plane
	///
	///	defines two "half spaces"
	template<typename T, size_t DIM, typename S, typename Q> struct Plane
	{
		VECTOR(T, DIM, S, QUANTITY_PURE) n;
		SCALAR(T, S, Q) d;

		Plane() = default;

		Plane(VECTOR(T, DIM, S, QUANTITY_PURE) const &n, SCALAR(T, S, Q) const &d)
			: n(n)
			, d(d)
		{
		}

		~Plane() = default;

	};

}	///	math
}	///	lucid

///	TBD: operators...

namespace lucid {
namespace math {

	template<typename T, size_t DIM, typename S, typename Q>
	inline SCALAR(T, S, Q) dot(PLANE(T, DIM, S, Q) const &lhs, VECTOR(T, DIM, S, Q) const &rhs)
	{
		return dot(lhs.n, rhs) + lhs.d;
	}

	template<typename T, typename S, typename Q>
	inline PLANE(T, 3, S, Q) makePlane3(VECTOR(T, 3, S, Q) const &a, VECTOR(T, 3, S, Q) const &b, VECTOR(T, 3, S, Q) const &c)
	{
		VECTOR(T, 3, S, QUANTITY_PURE) normal = normalize(cross(b - a, c - a));
		return PLANE(T, 3, S, Q)(normal, -dot(normal, a));
	}

}	///	math
}	///	lucid

///
///
///

#undef PLANE
#pragma pop_macro("PLANE")

#undef VECTOR
#pragma pop_macro("VECTOR")

#undef SCALAR
#pragma pop_macro("SCALAR")

#undef QUANTITY_PURE
#pragma pop_macro("QUANTITY_PURE")
