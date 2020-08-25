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
	orbit::scalar_t const seconds_per_day = orbit::constants::seconds_per_day<orbit::scalar_t>();
	orbit::scalar_t const meters_per_au = orbit::constants::meters_per_au<orbit::scalar_t>();
	orbit::scalar_t const au_per_meters = 1.0 / meters_per_au;
	orbit::scalar_t const meters_per_ru = meters_per_au / 100.0;
	orbit::scalar_t const ru_per_meters = 1.0 / meters_per_ru;

	struct SphereInstance
	{
		gal::Color color;
		gal::Vector4 position;
	};

}	///	anonymous

///
///
///

void OrbitTest::begin(float64_t t)
{
	::log("INFO", "starting orbit test...");

	LUCID_PROFILE_BEGIN("begin orbit test");

	_context = gigl::Context("content/test.context");

	_sphereMesh = gigl::Resources::get<gigl::Mesh>("content/sphere.mesh");
	_sphereInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, SPHERE_MAXIMUM, sizeof(SphereInstance)));

	_maskMesh = gigl::Resources::get<gigl::Mesh>("content/orbitMask.mesh");
	_maskInstances.reset(gal::VertexBuffer::create(gal::VertexBuffer::USAGE_DYNAMIC, SPHERE_MAXIMUM, sizeof(Vector4)));

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

	Body ssb;
	ssb.name = "SSB";
	ssb.position = orbit::vector3_t(0, 0, 0);
	ssb.velocity = orbit::vector3_t(0, 0, 0);
	ephemeris.lookup(ssb.properties, ssb.name);
	_bodies.insert(std::make_pair(0, ssb));

	addBody(    "Sun", 2451544.0);
	addBody("Mercury", 2451544.0);
	addBody(  "Venus", 2451544.0);
	addBody(  "Earth", 2451544.0);
	addBody(   "Moon", 2451544.0);
	addBody(   "Mars", 2451544.0);
	addBody("Jupiter", 2451544.0);
	addBody( "Saturn", 2451544.0);
	addBody( "Uranus", 2451544.0);
	addBody("Neptune", 2451544.0);
	addBody(  "Pluto", 2451544.0);

	LUCID_PROFILE_END();

	///	initial view positon/direction
	_viewPosition = Vector3(75, 75, 20);
	_viewDirection = math::normalize(Vector3( 0,  0,  0) - _viewPosition);

	LUCID_PROFILE_END();
}

void OrbitTest::onInput(MouseEvent const &event)
{
	if (event.btnDownLeft)
	{
		float32_t theta = 0.01f * event.x;
		float32_t   phi = 0.01f * event.y;
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

	Vector3 e1 = _viewDirection;
	Vector3 e2 = Vector3(0, 0, 1);
	Vector3 e0 = math::cross(_viewDirection, e2);
	        e2 = math::cross(e0, e1);

	Matrix4x4 viewMatrix = math::look(_viewPosition, _viewPosition + 5.f * _viewDirection, e2);
	Matrix4x4 projMatrix = math::perspective(fov, aspect, 1.f, 1000.f);

	Matrix4x4 invViewMatrix = math::inverse(viewMatrix);

	_context[   "viewRight"] = e0;
	_context[ "viewForward"] = e1;
	_context[      "viewUp"] = e2;
	_context["viewPosition"] = _viewPosition;

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

	pipeline.clear(true, true, true, gal::Color(0, 0, 0, 1));;

	renderBodies(time, interpolant);
	renderMasks(time, interpolant);
	renderOrbits(time, interpolant);

	LUCID_PROFILE_END();
}

void OrbitTest::addBody(std::string const &name, float32_t jdn)
{
	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();

	Body target;
	target.name = name;
	target.  id = ephemeris.lookup(name);
	              ephemeris.lookup(target.properties, target.id);
	target. cid = ephemeris.lookup(target.  elements, target.id, jdn);
	LUCID_VALIDATE(_bodies.end() == _bodies.find(target.id), "duplicate body specified");

	auto iter = _bodies.find(target.cid);
	LUCID_VALIDATE(iter != _bodies.end(), "unknown center body");
	Body const &center = iter->second;

	ephemeris.compute(target.position, target.velocity, center.properties, target.elements, jdn);

	_bodies.insert(std::make_pair(target.id, target));
}

void OrbitTest::renderOrbits(float32_t time, float32_t interpolant) const
{
	LUCID_PROFILE_BEGIN("render orbits");

	for (auto iter = _bodies.begin(); iter != _bodies.end(); ++iter)
	{
		Body const &body = iter->second;
		if (0 == body.id)
			continue;
		renderOrbit(body);
	}

	LUCID_PROFILE_END();
}

void OrbitTest::renderOrbit(Body const &target) const
{
	LUCID_PROFILE_BEGIN("render orbit");

	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();


	orbit::Ephemeris const &ephemeris = orbit::Ephemeris::instance();
	orbit::matrix3x3_t R;
	ephemeris.compute(R, target.elements);

	///	TBD: add position of center body
	gal::Matrix4x4 worldMatrix(
		R.xx, R.xy, R.xz,    0,
		R.yx, R.yy, R.yz,    0,
		R.zx, R.zy, R.zz,    0,
		   0,    0,    0,    1
	);
	gal::Vector2 domain(-math::constants::pi<float32_t>(), math::constants::pi<float32_t>());
	float32_t a = float32_t(target.elements.A * ru_per_meters);
	float32_t e = float32_t(target.elements.EC);

	std::shared_ptr<gigl::Material> material = _orbitMesh->material();
	std::shared_ptr<gal::Program> program = material->program();

	///	with e < 1 one can use hu = a * (1 - e^2)
	/// with a == semi-major axis
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

void OrbitTest::renderMasks(float32_t time, float32_t interpolant) const
{
	LUCID_PROFILE_BEGIN("render masks");

	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();

	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	Vector4 *instances = (Vector4 *)_maskInstances->lock();
	int32_t count = 0;
	for (auto iter = _bodies.begin(); iter != _bodies.end(); ++iter)
	{
		Body const &body = iter->second;
		if (0 == body.id)
			continue;

		orbit::vector3_t position = body.position * ru_per_meters;
		orbit::vector3_t velocity = body.velocity * ru_per_meters;

		instances[count] = gal::Vector4(float32_t(position.x), float32_t(position.y), float32_t(position.z), 5);
		++count;
	}
	_maskInstances->unlock();

	std::shared_ptr<gigl::Material> material = _maskMesh->material();
	material->begin(_context);
	pipeline.setVertexStream(1, _maskInstances.get());
		_maskMesh->drawInstanced(count);
	material->end();

	LUCID_PROFILE_END();
}

void OrbitTest::renderBodies(float32_t time, float32_t interpolant) const
{
	LUCID_PROFILE_BEGIN("render bodies");

	orbit::Ephemeris &ephemeris = orbit::Ephemeris::instance();

	gal::Pipeline &pipeline = lucid::gal::Pipeline::instance();

	SphereInstance *instances = (SphereInstance *)_sphereInstances->lock();
	int32_t count = 0;
	for (auto iter = _bodies.begin(); iter != _bodies.end(); ++iter)
	{
		Body const &body = iter->second;
		if (0 == body.id)
			continue;

		orbit::vector3_t position = body.position * ru_per_meters;
		orbit::vector3_t velocity = body.velocity * ru_per_meters;

		instances[count].color = gal::Color(0, 1, 0, 1);
		instances[count].position = gal::Vector4(float32_t(position.x), float32_t(position.y), float32_t(position.z), 5);
		++count;
	}
	_sphereInstances->unlock();

	std::shared_ptr<gigl::Material> material = _sphereMesh->material();
	material->begin(_context);
	pipeline.setVertexStream(1, _sphereInstances.get());
		_sphereMesh->drawInstanced(count);
	material->end();

	LUCID_PROFILE_END();
}
