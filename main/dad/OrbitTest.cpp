#include "OrbitTest.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Elements.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Constants.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/String.h>

///
///
///

namespace constants = lucid::math::constants;

namespace  core = ::lucid::core;
namespace   gal = ::lucid::gal;
namespace  gigl = ::lucid::gigl;
namespace  math = ::lucid::math;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */
{

}	///	anonymous

///
///
///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	LUCID_PROFILE_BEGIN("ephemeris test");

	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();
	ephemeris.initialize("content/j2000.ephemeris");

	ephemeris.lookup(_bodyProperties, "Sun");
	ephemeris.lookup(_orbitElements, "Earth", 2451544.5f);

	/// test {
	///		get the position and velocity of earth at J2000.5 (2451544.5) then
	///		convert to au and au/day to verify against Horizons
	orbit::scalar_t const meters_per_au = orbit::constants::meters_per_au<orbit::scalar_t>();
	orbit::scalar_t const seconds_per_day = orbit::constants::seconds_per_day<orbit::scalar_t>();

	orbit::vector3_t position;
	orbit::vector3_t velocity;
	ephemeris.compute(position, velocity, "Earth", 2451544.5f);

	position = position / meters_per_au;
	velocity = seconds_per_day * velocity / meters_per_au;
	///		verifying manually...  looks good.
	///		rounding error with such large numbers, for now, is problematic.
	/// } test

	LUCID_PROFILE_END();

	_context = gigl::Context("content/test.context");
	_orbit   = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");

	///	initial view positon/direction
	_viewPosition = Vector3(10, 10, 10);
	_viewDirection = math::normalize(Vector3( 0,  0,  0) - _viewPosition);

	LUCID_PROFILE_END();
}

void OrbitTest::onInput(MouseEvent const &event)
{
	///	TBD: update view 
}

bool OrbitTest::update(float64_t t, float64_t dt)
{
	_passed = true;

	float32_t const fov = constants::half_pi<float32_t>();
	float32_t const aspect = gal::System::instance().aspect();

	Vector3 left = math::cross(Vector3(0, 0, 1), _viewDirection);
	Vector3   up = math::cross(_viewDirection, left);

	Matrix4x4 viewMatrix = math::look(_viewPosition, _viewPosition + 5.f * _viewDirection, up);
	Matrix4x4 projMatrix = math::perspective(fov, aspect, 1.f, 1000.f);

	Matrix4x4 invViewMatrix = math::inverse(viewMatrix);

	Vector4 e0 = invViewMatrix * Vector4(1, 0, 0, 0);
	Vector4 e2 = invViewMatrix * Vector4(0, 1, 0, 0);

	_context["viewRight"] = Vector3(e0.x, e0.y, e0.z);
	_context["viewUp"] = Vector3(e2.x, e2.y, e2.z);

	_context["viewMatrix"] = viewMatrix;
	_context["projMatrix"] = projMatrix;
	_context["viewProjMatrix"] = projMatrix * viewMatrix;

	///	return true to exit testing (false to continue)
	return false;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	LUCID_PROFILE_BEGIN("orbit rendering");

	_context["time"] = time;
	_context["interpolant"] = interpolant;

	///
	///	begin rendering
	///

	pipeline.clear(true, true, true, gal::Color(0, 0, 0, 1));;

	///
	///	render an orbit
	///

	{
		std::shared_ptr<gal::Program> program = _orbit->program();
		std::shared_ptr<gigl::Material> material = _orbit->material();

		material->begin(_context);
			_orbit->draw();
		material->end();
	}

	LUCID_PROFILE_END();
}
