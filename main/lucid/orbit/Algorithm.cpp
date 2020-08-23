#include "Algorithm.h"
#include <lucid/math/Constants.h>
#include <cmath>

namespace math = ::lucid::math;

///
///
///

namespace lucid {
namespace orbit {

	void computePositionVelocity(vector3_t &position, vector3_t &velocity, Properties const &centerProperties, Elements const &targetElements, scalar_t jdn)
	{
		scalar_t const twopi = math::constants::two_pi<scalar_t>();
		scalar_t const tolsq = math::constants::tol_tol<scalar_t>();
		scalar_t const    dt = constants::seconds_per_day<scalar_t>() * (jdn - targetElements.JDN);
		scalar_t const    GM = centerProperties.GM;
		scalar_t const     e = targetElements.EC;
		scalar_t const     a = targetElements.A;

		scalar_t MA = targetElements.MA + dt * ::sqrt(GM / ::pow(a, 3.f));
		MA = std::fmod(MA, twopi);
		MA = (MA < 0.f) ? MA + twopi: MA;

		scalar_t EA[2] = { MA, 0.f };

		size_t const limit = 10;
		size_t iter = 0;

		scalar_t err = EA[0] - EA[1];
		while (((err * err) > tolsq) && (iter < limit))
		{
			EA[1] = EA[0] - (EA[0] - e * ::sin(EA[0]) - MA) / (1.f - e * ::cos(EA[0]));
			err = EA[1] - EA[0];

			std::swap(EA[0], EA[1]);
			++iter;
		}

		scalar_t TA = 2.f * ::atan2(::sqrt(1.f + e) * ::sin(0.5f * EA[0]), ::sqrt(1.f - e) * ::cos(0.5f * EA[0]));
		scalar_t  r = a * (1.f - e * ::cos(EA[0]));

		position = r * vector3_t(::cos(TA), ::sin(TA), 0.f);
		velocity = ::sqrt(GM * a) / r * vector3_t(-::sin(EA[0]), ::sqrt(1.f - e * e) * ::cos(EA[0]), 0.f);

		matrix3x3_t R = rotationMatrixFrom(targetElements);

		position = R * position;
		velocity = R * velocity;
	}

}	///	orbit
}	///	lucid
