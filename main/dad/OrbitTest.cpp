#include "OrbitTest.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/orbit/Frame.h>
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/StarCatalog.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Utility.h>
#include <lucid/gal/System.h>
#include <lucid/math/Vector.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/String.h>

#include <lucid/gigl/Mesh.h>

/// 
/// 
/// 

LUCID_ANONYMOUS_BEGIN

LUCID_ANONYMOUS_END

/// 
/// 
/// 

void OrbitTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	LUCID_ORBIT_STARCATALOG.initialize("content/bsc5.starcatalog");
	LUCID_ORBIT_EPHEMERIS.initialize("content/j2000.ephemeris");

	_orbitalMechanics = new LUCID_ORBIT::Mechanics(LUCID_ORBIT::JDN::now());
	_cameraFrame = new LUCID_ORBIT::CameraFrame(1001, "camaera", "");

	_orbitalMechanics->attach(_orbitalMechanics->frame(399), _cameraFrame);
	_cameraFrame->relativePosition[0] = _cameraFrame->relativePosition[1] = LUCID_ORBIT::vector3_t(10, 10, 5);
	_cameraFrame->look(_orbitalMechanics->frame(499));

	LUCID_PROFILE_END();
}

void OrbitTest::onInput(MouseEvent const &event)
{
}

bool OrbitTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	_orbitalMechanics->update();

	///	return true to exit testing (false to continue)
	return false;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	LUCID_PROFILE_BEGIN("orbit rendering");

	_orbitalMechanics->render(_cameraFrame);

	LUCID_PROFILE_END();
}
