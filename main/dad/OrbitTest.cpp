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

namespace  core = ::lucid:: core;
namespace   gal = ::lucid::  gal;
namespace  gigl = ::lucid:: gigl; 
namespace  math = ::lucid:: math;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */
{

	inline orbit::StarCatalog &theStarCatalog()
	{
		return orbit::StarCatalog::instance();
	}

	inline orbit::Ephemeris &theEphemeris()
	{
		return orbit::Ephemeris::instance();
	}

}	///	anonymous

///
///
///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	theStarCatalog().initialize("content/bsc5.starcatalog");
	theEphemeris().initialize("content/j2000.ephemeris");

	_orbitalSystem.initialize(orbit::constants::J2000<orbit::scalar_t>());
	_cameraFrame = new orbit::CameraFrame(1001, "camaera", "");
	
	_orbitalSystem.attach(_orbitalSystem.root(), _cameraFrame);

	LUCID_PROFILE_END();
}

void OrbitTest::onInput(MouseEvent const &event)
{
}

bool OrbitTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	float32_t const    fov = math::constants::quarter_pi<float32_t>();
	float32_t const aspect = gal::System::instance().aspect();

	gigl::Camera2D &camera = _cameraFrame->camera;
	camera.initPerspective(fov, aspect, 1.f, 1000.f);

	_orbitalSystem.update();

	///	return true to exit testing (false to continue)
	return false;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	gal::Pipeline &pipeline = gal::Pipeline::instance();

	LUCID_PROFILE_BEGIN("orbit rendering");

	_orbitalSystem.render(_cameraFrame);

	LUCID_PROFILE_END();
}
