#pragma once

#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
#include <lucid/math/Matrix.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Elements.h>

namespace lucid {
namespace orbit {

	inline matrix3x3_t rotationMatrixFrom(Elements const &targetElements)
	{
		return ::lucid::math::rotateAboutZ(targetElements.OM) * ::lucid::math::rotateAboutX(targetElements.IN) * ::lucid::math::rotateAboutZ(targetElements.W);
	}

	void computePositionVelocity(vector3_t &position, vector3_t &velocity, Properties const &centerProperties, Elements const &targetElements, scalar_t jdn);

}	///	orbit
}	///	lucid
