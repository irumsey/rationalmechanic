#pragma once

#include <cmath>
#include <lucid/math/Scalar.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

///
///
///

namespace lucid {
namespace orbit {

	///
	///
	///

	inline gal::Scalar cast(scalar_t rhs)
	{
		return gal::Scalar(float32_t(rhs.value));
	}

	inline scalar_t cast(gal::Scalar rhs)
	{
		return scalar_t(float64_t(rhs.value));
	}

	inline gal::Scalar scale(scalar_t const &rhs)
	{
		return cast(constants::RUs_per_meter<float64_t> * rhs);
	}

	inline scalar_t inv_scale(gal::Scalar const &rhs)
	{
		return cast(constants::meters_per_RU<float32_t> * rhs);
	}

	inline gal::Vector3 cast(vector3_t const &rhs)
	{
		return gal::Vector3(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z));
	}

	inline vector3_t cast(gal::Vector3 const &rhs)
	{
		return vector3_t(float64_t(rhs.x), float64_t(rhs.y), float64_t(rhs.z));
	}

	inline gal::Vector3 scale(vector3_t const &rhs)
	{
		return cast(constants::RUs_per_meter<float64_t> * rhs);
	}

	inline vector3_t inv_scale(gal::Vector3 const &rhs)
	{
		return cast(constants::meters_per_RU<float32_t> * rhs);
	}

	inline gal::Matrix3x3 cast(matrix3x3_t const &rhs)
	{
		return gal::Matrix3x3(
			float32_t(rhs.xx), float32_t(rhs.xy), float32_t(rhs.xz),
			float32_t(rhs.yx), float32_t(rhs.yy), float32_t(rhs.yz),
			float32_t(rhs.zx), float32_t(rhs.zy), float32_t(rhs.zz)
		);
	}

	///
	/// 
	/// 
	inline vector2_t computeConicPoint(scalar_t hu, scalar_t e, scalar_t theta)
	{
		scalar_t c = math::cos(theta);
		scalar_t s = math::sin(theta);
		scalar_t r = hu / (scalar_t(1.0) + e * c);

		return r * vector2_t(c, s);
	}

	///
	///
	///
	inline matrix3x3_t rotationFromElements(Elements const &elements)
	{
		return math::rotateAboutZ(elements.OM) * math::rotateAboutX(elements.IN) * math::rotateAboutZ(elements.W);
	}

	///
	///
	///
	inline void kinematicsFromElements(vector3_t &position, vector3_t &velocity, PhysicalProperties const &centerProperties, Elements const &targetElements, scalar_t jdn)
	{
		scalar_t const twopi = constants::two_pi<float64_t>;
		scalar_t const tolsq = constants::tolsq<float64_t>;
		scalar_t const    dt = constants::seconds_per_day<float64_t> * (jdn - targetElements.JDN);
		scalar_t const    GM = centerProperties.GM;
		scalar_t const     e = targetElements.EC;
		scalar_t const     a = targetElements.A;

		scalar_t MA = targetElements.MA + dt * std::sqrt(GM / std::pow(a, 3.0));
		MA = std::fmod(MA, twopi);
		MA = (MA < 0.0) ? MA + twopi: MA;

		scalar_t EA[2] = { MA, 0.0 };

		size_t const limit = 10;
		size_t iter = 0;

		scalar_t err = EA[0] - EA[1];
		while (((err * err) > tolsq) && (iter < limit))
		{
			EA[1] = EA[0] - (EA[0] - e * std::sin(EA[0]) - MA) / (1.0 - e * std::cos(EA[0]));
			err = EA[1] - EA[0];

			std::swap(EA[0], EA[1]);
			++iter;
		}

		scalar_t TA = scalar_t(2.0) * math::atan2(math::sqrt(scalar_t(1.0) + e) * math::sin(scalar_t(0.5) * EA[0]), math::sqrt(scalar_t(1.0) - e) * math::cos(scalar_t(0.5) * EA[0]));
		scalar_t  r = a * (1.0 - e * math::cos(EA[0]));

		position = r * vector3_t(math::cos(TA), math::sin(TA), scalar_t(0.0));
		velocity = math::sqrt(GM * a) / r * vector3_t(-math::sin(EA[0]), math::sqrt(scalar_t(1.0) - e * e) * math::cos(EA[0]), scalar_t(0.0));

		matrix3x3_t R = rotationFromElements(targetElements);

		position = R * position;
		velocity = R * velocity;
	}

}	///	orbit
}	///	lucid
