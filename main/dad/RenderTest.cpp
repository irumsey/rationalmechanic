#include "RenderTest.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/gigl/Mesh.h>
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gal/System.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/IndexBuffer.h>
#include <lucid/gal/VertexBuffer.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Numbers.h>
#include <sstream>
#include <algorithm>

///
///
///

LUCID_ANONYMOUS_BEGIN

inline float32_t random(float32_t a, float32_t b)
{
	float32_t t = 0.01f * (::rand() % 101);
	return (b - a) * t + a;
}

LUCID_ANONYMOUS_END

///
///
///

float64_t const RenderTest::DURATION = 30.0;
float64_t const RenderTest::EMIT_RATE = 11.0;

void RenderTest::begin(float64_t t)
{
	LUCID_CORE::log("INFO", "starting basic render test...");

	_timeStart = t;
	_frameCount = 0;

	_context = LUCID_GIGL::Context("content/test.context");
	_mesh = LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>("content/particle.mesh");

	_instances.reset(LUCID_GAL::VertexBuffer::create(LUCID_GAL::VertexBuffer::USAGE_DYNAMIC, PARTICLE_MAXIMUM, sizeof(Particle)));
}

void RenderTest::onInput(MouseEvent const &event)
{
	///	TBD: update view
}

bool RenderTest::update(float64_t t, float64_t dt)
{
	emitParticles((float32_t)t, (float32_t)dt);

	updateParticles((float32_t)t, (float32_t)dt);

	if ((t - _timeStart) > DURATION)
	{
		float64_t fps = _frameCount / (t - _timeStart);
		
		std::ostringstream os;
		os << "FPS = " << fps;

		LUCID_CORE::log("INFO", os.str());

		return true;
	}

	return false;
}

void RenderTest::render(float32_t time, float32_t interpolant)
{
	float32_t const fov = 0.25f * LUCID_CORE_NUMBERS::pi<float32_t>;
	float32_t const aspect =LUCID_GAL_SYSTEM.aspect();

	///
	///	in a "real" application, setting up global render information (context) would be
	///	a once per-frame operation.  however, this is just a test.
	///

	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0.9f, 0.9f, 0.9f, 1));

	Matrix4x4 viewMatrix = LUCID_MATH::look(Vector3(-20.f, 0, 30.f), Vector3(-20.f, 0, 0), Vector3(0, 1, 0));
	Matrix4x4 projMatrix = LUCID_MATH::perspective(fov, aspect, 1.f, 1000.f);

	Matrix4x4 invViewMatrix = LUCID_MATH::inverse(viewMatrix);

	Vector4 e0 = invViewMatrix * Vector4(1, 0, 0, 0);
	Vector4 e2 = invViewMatrix * Vector4(0, 1, 0, 0);

	_context["time"] = time;
	_context["interpolant"] = interpolant;

	_context["viewRight"] = Vector3(e0.x, e0.y, e0.z);
	_context["viewUp"] = Vector3(e2.x, e2.y, e2.z);

	_context["viewMatrix"] = viewMatrix;
	_context["projMatrix"] = projMatrix;
	_context["viewProjMatrix"] = projMatrix * viewMatrix;

	///
	///	render smoke particles
	///
	int32_t count = int32_t(_particles.size());
	if (0 < count)
	{
		std::shared_ptr<LUCID_GAL::Program> program = _mesh->program();
		std::shared_ptr<LUCID_GIGL::Material> material = _mesh->material();

		material->begin(_context);
				LUCID_GAL_PIPELINE.setVertexStream(1, _instances.get());
				_mesh->drawInstanced(count);
		material->end();
	}

	++_frameCount;
}

void RenderTest::emitParticles(float32_t t, float32_t dt)
{
	_emitAccum += dt * EMIT_RATE;
	int32_t count = (int32_t)std::floor(_emitAccum);
	_emitAccum = _emitAccum - count;

	for (int32_t i = 0; i < count; ++i)
	{
		if (_particles.size() < PARTICLE_MAXIMUM)
		{
			emitParticle(t, dt);
		}
	}
}

void RenderTest::emitParticle(float32_t t, float32_t dt)
{
	Particle particle;

	particle.timeEmit = t;
	particle.timeSpan = random(11.f, 15.f);

	particle.position[0] = Vector3(random(-0.1f, 0.1f), random(-0.1f, 0.1f), 0.f);
	particle.position[1] = Vector3(random(-0.3f, 0.3f), random(-0.3f, 0.3f), random(3.f, 5.f)) * dt + particle.position[0];

	particle.rotation[0] = random(-LUCID_CORE_NUMBERS::pi<float32_t>, LUCID_CORE_NUMBERS::pi<float32_t>);
	particle.rotation[1] = random(-0.9f, 0.9f) * dt + particle.rotation[0];

	particle.scale[0] = random(0.77f, 1.3f);
	particle.scale[1] = random(0.5f, 1.1f) * dt + particle.scale[0];

	Vector2 const texcoord[] =
	{
		{ 0.0f, 0.0f, },
		{ 0.0f, 0.5f, },
		{ 0.5f, 0.0f, },
		{ 0.5f, 0.5f, },
	};

	particle.texcoord = texcoord[::rand() % 4];
	particle.texsize = Vector2(0.5f, 0.5f);

	_particles.push_back(particle);
}

void RenderTest::updateParticles(float32_t t, float32_t dt)
{
	///
	///
	///
	auto updateParticle = [t, dt](Particle &particle)
	{
		if ((t - particle.timeEmit) >= particle.timeSpan)
		{
			return true;
		}

		Vector3 const wind = Vector3(0.f, -1.1f, 0.1f);
		float32_t const drag = 0.011f;

		particle.position[0] = (2.f - drag) * particle.position[1] - (1.f - drag) * particle.position[0] + wind * dt * dt;
		std::swap(particle.position[0], particle.position[1]);

		particle.rotation[0] = (2.f - drag) * particle.rotation[1] - (1.f - drag) * particle.rotation[0];
		std::swap(particle.rotation[0], particle.rotation[1]);

		particle.scale[0] = (2.f - drag) * particle.scale[1] - (1.f - drag) * particle.scale[0];
		std::swap(particle.scale[0], particle.scale[1]);

		return false;
	};

	_particles.erase(std::remove_if(_particles.begin(), _particles.end(), updateParticle), _particles.end());

	int32_t count = int32_t(_particles.size());
	if (0 < count)
	{
		::memcpy(_instances->lock(), &_particles[0], count * sizeof(Particle));
		_instances->unlock();
	}
}
