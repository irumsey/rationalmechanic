#pragma once

#include <cmath>
#include <lucid/math/Algorithm.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/gal/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

LUCID_ORBIT_BEGIN

///
///
///

inline float32_t cast(scalar_t rhs)
{
	return float32_t(rhs);
}

inline scalar_t cast(float32_t rhs)
{
	return scalar_t (rhs);
}

inline LUCID_GAL::Vector2 cast(vector2_t const &rhs)
{
	return LUCID_GAL::Vector2(float32_t(rhs.x), float32_t(rhs.y));
}

inline LUCID_GAL::Vector3 cast(vector3_t const &rhs)
{
	return LUCID_GAL::Vector3(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z));
}

inline vector3_t cast(LUCID_GAL::Vector3 const &rhs)
{
	return vector3_t(scalar_t(rhs.x), scalar_t(rhs.y), scalar_t(rhs.z));
}

inline LUCID_GAL::Matrix3x3 cast(matrix3x3_t const &rhs)
{
	return LUCID_GAL::Matrix3x3(
		float32_t(rhs.xx), float32_t(rhs.xy), float32_t(rhs.xz),
		float32_t(rhs.yx), float32_t(rhs.yy), float32_t(rhs.yz),
		float32_t(rhs.zx), float32_t(rhs.zy), float32_t(rhs.zz)
	);
}

inline LUCID_GAL::Matrix4x4 cast(matrix4x4_t const &rhs)
{
	return LUCID_GAL::Matrix4x4(
		float32_t(rhs.xx), float32_t(rhs.xy), float32_t(rhs.xz), float32_t(rhs.xw),
		float32_t(rhs.yx), float32_t(rhs.yy), float32_t(rhs.yz), float32_t(rhs.yw),
		float32_t(rhs.zx), float32_t(rhs.zy), float32_t(rhs.zz), float32_t(rhs.zw),
		float32_t(rhs.wx), float32_t(rhs.wy), float32_t(rhs.wz), float32_t(rhs.ww)
	);
}

inline LUCID_GAL::Quaternion cast(quaternion_t const &rhs)
{
	return LUCID_GAL::Quaternion(float32_t(rhs.x), float32_t(rhs.y), float32_t(rhs.z), float32_t(rhs.w));
}

///
/// 
/// 
inline vector2_t computeConicPoint(scalar_t hu, scalar_t e, scalar_t theta)
{
	scalar_t c = LUCID_MATH::cos(theta);
	scalar_t s = LUCID_MATH::sin(theta);
	scalar_t r = hu / (1.0 + e * c);

	return r * vector2_t(c, s);
}

///
///
///
inline matrix3x3_t rotationFromElements(Elements const &elements)
{
	return LUCID_MATH::rotateAboutZ(elements.OM) * LUCID_MATH::rotateAboutX(elements.IN) * LUCID_MATH::rotateAboutZ(elements.W);
}

///
///
///
inline void kinematicsFromElements(vector3_t &position, vector3_t &velocity, PhysicalProperties const &centerProperties, Elements const &targetElements, scalar_t jdn)
{
	scalar_t const twopi = constants::two_pi<scalar_t>;
	scalar_t const tolsq = constants::tolsq<scalar_t>;
	scalar_t const    dt = constants::seconds_per_day<scalar_t> * (jdn - targetElements.JDN);
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

	scalar_t TA = scalar_t(2.0) * LUCID_MATH::atan2(LUCID_MATH::sqrt(scalar_t(1.0) + e) * LUCID_MATH::sin(scalar_t(0.5) * EA[0]), LUCID_MATH::sqrt(scalar_t(1.0) - e) * LUCID_MATH::cos(scalar_t(0.5) * EA[0]));
	scalar_t  r = a * (1.0 - e * LUCID_MATH::cos(EA[0]));

	position = r * vector3_t(LUCID_MATH::cos(TA), LUCID_MATH::sin(TA), scalar_t(0.0));
	velocity = LUCID_MATH::sqrt(GM * a) / r * vector3_t(-LUCID_MATH::sin(EA[0]), LUCID_MATH::sqrt(scalar_t(1.0) - e * e) * LUCID_MATH::cos(EA[0]), scalar_t(0.0));

	matrix3x3_t R = rotationFromElements(targetElements);

	position = R * position;
	velocity = R * velocity;
}

LUCID_ORBIT_END