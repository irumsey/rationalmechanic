#include "OrbitTest.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/orbit/Ephemeris.h>
#include <lucid/orbit/Algorithm.h>
#include <lucid/orbit/Elements.h>
#include <lucid/orbit/Properties.h>
#include <lucid/orbit/Constants.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/core/Profiler.h>
#include <lucid/core/String.h>

///
///
///

namespace  core = ::lucid::core;
namespace   gal = ::lucid::gal;
namespace  gigl = ::lucid::gigl;
namespace  math = ::lucid::math;
namespace orbit = ::lucid::orbit;

namespace /* anonymous */
{
	///	ru = render length unit
	orbit::scalar_t const meters_per_ru = 100.0 / orbit::constants::meters_per_au<float32_t>();

}	///	anonymous

///
///
///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	_context = gigl::Context("content/test.context");

	_orbitMesh = gigl::Resources::get<gigl::Mesh>("content/orbit.mesh");

	std::shared_ptr<gigl::Material> material = _orbitMesh->material();
	std::shared_ptr<gal::Program> program = material->program();

	_renderParameters.          hu = program->lookup(          "hu");
	_renderParameters.eccentricity = program->lookup("eccentricity");
	_renderParameters.      domain = program->lookup(      "domain");
	_renderParameters.   lineColor = program->lookup(   "lineColor");
	_renderParameters. worldMatrix = program->lookup( "worldMatrix");

	LUCID_PROFILE_BEGIN("ephemeris test");

	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();
	ephemeris.initialize("content/j2000.ephemeris");

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
	/// } test

	LUCID_PROFILE_END();

	///	initial view positon/direction
	_viewPosition = Vector3(80, 80, 15);
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

	float32_t const fov = math::constants::half_pi<float32_t>();
	float32_t const aspect = gal::System::instance().aspect();

	Vector3 left = math::cross(Vector3(0, 0, 1), _viewDirection);
	Vector3   up = math::cross(_viewDirection, left);

	Matrix4x4 viewMatrix = math::look(_viewPosition, _viewPosition + 5.f * _viewDirection, up);
	Matrix4x4 projMatrix = math::perspective(fov, aspect, 1.f, 1000.f);

	Matrix4x4 invViewMatrix = math::inverse(viewMatrix);

	Vector4 e0 = invViewMatrix * Vector4(1, 0, 0, 0);
	Vector4 e2 = invViewMatrix * Vector4(0, 1, 0, 0);
	Vector4 e3 = invViewMatrix * Vector4(0, 0, 1, 0);

	_context["viewRight"] = Vector3(e0.x, e0.y, e0.z);
	_context["viewUp"] = Vector3(e2.x, e2.y, e2.z);
	_context["viewForward"] = Vector3(e3.x, e3.y, e3.z);

	_context["viewMatrix"] = viewMatrix;
	_context["projMatrix"] = projMatrix;
	_context["viewProjMatrix"] = projMatrix * viewMatrix;

	///	return true to exit testing (false to continue)
	return false;
}

void OrbitTest::render(float32_t time, float32_t interpolant)
{
	/// test {
	_viewPosition = _viewPosition - 0.03f * gal::Vector3(10, 10, 0);
	/// } test

	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	LUCID_PROFILE_BEGIN("orbit rendering");

	_context["time"] = time;
	_context["interpolant"] = interpolant;

	pipeline.clear(true, true, true, gal::Color(0, 0, 0, 1));;

	renderOrbit("Mercury", 2451544.5f);
	renderOrbit(  "Venus", 2451544.5f);
	renderOrbit(  "Earth", 2451544.5f);
	renderOrbit(   "Mars", 2451544.5f);
	renderOrbit("Jupiter", 2451544.5f);

	LUCID_PROFILE_END();
}

void OrbitTest::renderOrbit(std::string const &target, float32_t jdn) const
{
	LUCID_PROFILE_BEGIN("render orbit");

	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();

	orbit::Elements orbitElements;
	ephemeris.lookup(orbitElements, target, jdn);

	orbit::matrix3x3_t R = orbit::rotationMatrixFrom(orbitElements);
	gal::Matrix4x4 worldMatrix(
		R.xx, R.xy, R.xz,    0,
		R.yx, R.yy, R.yz,    0,
		R.zx, R.zy, R.zz,    0,
		0,    0,    0,    1
		);
	gal::Vector2 domain(-math::constants::pi<float32_t>(), math::constants::pi<float32_t>());
	float32_t a = float32_t(orbitElements.A * meters_per_ru);
	float32_t e = float32_t(orbitElements.EC);

	std::shared_ptr<gigl::Material> material = _orbitMesh->material();
	std::shared_ptr<gal::Program> program = material->program();

	///	with e < 1 one can use hu = a * (1 - e^2)
	/// with a = semi-major axis
	program->set(_renderParameters.          hu,      a * (1.f - e * e));
	program->set(_renderParameters.eccentricity,                      e);
	program->set(_renderParameters.      domain,                 domain);
	program->set(_renderParameters.   lineColor, gal::Color(0, 0, 1, 1));
	program->set(_renderParameters. worldMatrix,            worldMatrix);

	material->begin(_context);
	_orbitMesh->draw();
	material->end();

	LUCID_PROFILE_END();
}
