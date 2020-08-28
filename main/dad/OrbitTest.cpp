#include "OrbitTest.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Constants.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/math/Vector.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/String.h>

///
///
///

namespace  core = ::lucid:: core;
namespace  math = ::lucid:: math;
namespace   gal = ::lucid::  gal;
namespace  gigl = ::lucid:: gigl;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */
{

	inline ::lucid::orbit::Ephemeris &theEphemeris()
	{
		return ::lucid::orbit::Ephemeris::instance();
	}

}	///	anonymous

///
///
///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	_context = gigl::Context("content/test.context");

	LUCID_PROFILE_BEGIN("ephemeris test");

	theEphemeris().initialize("content/j2000.ephemeris");
	_orbitalSystem.initialize(orbit::constants::j2000<orbit::scalar_t>());

	LUCID_PROFILE_END();

	///	initial view positon/direction
	_viewPosition = gal::Vector3(75, 75, 20);
	_viewDirection = math::normalize(gal::Vector3( 0,  0,  0) - _viewPosition);

	LUCID_PROFILE_END();
}

void OrbitTest::onInput(MouseEvent const &event)
{
	if (event.btnDownLeft)
	{
		float32_t theta = 0.03f * event.x;
		float32_t   phi = 0.03f * event.y;
		_viewDirection = math::normalize(math::rotateAboutX(phi) * math::rotateAboutZ(theta) * _viewDirection);
	}

	if (event.btnDownRight)
	{
		_viewPosition = -0.3f * event.y * _viewDirection + _viewPosition;
	}
}

bool OrbitTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	float32_t const    fov = math::constants::quarter_pi<float32_t>();
	float32_t const aspect = gal::System::instance().aspect();

	gal::Vector3 e1 = _viewDirection;
	gal::Vector3 e2 = gal::Vector3(0, 0, 1);
	gal::Vector3 e0 = math::cross(_viewDirection, e2);
	             e2 = math::cross(e0, e1);

	gal::Matrix4x4 viewMatrix = math::look(_viewPosition, _viewPosition + 5.f * _viewDirection, e2);
	gal::Matrix4x4 projMatrix = math::perspective(fov, aspect, 1.f, 1000.f);

	gal::Matrix4x4 invViewMatrix = math::inverse(viewMatrix);

	_context[   "viewRight"] = e0;
	_context[ "viewForward"] = e1;
	_context[      "viewUp"] = e2;
	_context["viewPosition"] = _viewPosition;

	_context["viewMatrix"] = viewMatrix;
	_context["projMatrix"] = projMatrix;
	_context["viewProjMatrix"] = projMatrix * viewMatrix;

	///	dt is in seconds.  however, orbit::System::update() takes
	///	a time step of days.  therefore, if dt is 0.1 that translates
	///	to 8640 seconds (where 86400s is one day).  this happens 10 times
	///	a second, so, 1 day ticks off per second on screen.
	_orbitalSystem.update(dt);

	///	return true to exit testing (false to continue)
	return false;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	LUCID_PROFILE_BEGIN("orbit rendering");

	_context["time"] = time;
	_context["interpolant"] = interpolant;

	pipeline.clear(true, true, true, gal::Color(0, 0, 0, 1));;

	_orbitalSystem.render(_context, time, interpolant);

	LUCID_PROFILE_END();
}

