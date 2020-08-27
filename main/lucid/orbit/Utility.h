#pragma once

#include <cmath>
#include <lucid/math/Constants.h>
#include <lucid/math/Matrix.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
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
	inline matrix3x3_t rotationFromElements(Elements const &elements)
	{
		return math::rotateAboutZ(elements.OM) * math::rotateAboutX(elements.IN) * math::rotateAboutZ(elements.W);
	}

	///
	///
	///
	inline void kinematicsFromElements(vector3_t &position, vector3_t &velocity, Properties const &centerProperties, Elements const &targetElements, scalar_t jdn)
	{
		scalar_t const twopi = lucid::math::constants::two_pi<scalar_t>();
		scalar_t const tolsq = lucid::math::constants::tol_tol<scalar_t>();
		scalar_t const    dt = constants::seconds_per_day<scalar_t>() * (jdn - targetElements.JDN);
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

		scalar_t TA = 2.0 * std::atan2(std::sqrt(1.0 + e) * std::sin(0.5 * EA[0]), std::sqrt(1.0 - e) * std::cos(0.5 * EA[0]));
		scalar_t  r = a * (1.0 - e * std::cos(EA[0]));

		position = r * vector3_t(std::cos(TA), std::sin(TA), 0.0);
		velocity = std::sqrt(GM * a) / r * vector3_t(-std::sin(EA[0]), std::sqrt(1.0 - e * e) * std::cos(EA[0]), 0.0);

		matrix3x3_t R = rotationFromElements(targetElements);

		position = R * position;
		velocity = R * velocity;
	}

}	///	orbit
}	///	lucid