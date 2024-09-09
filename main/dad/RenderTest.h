#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/math/Vector.h>
#include <lucid/math/Matrix.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/gigl/Context.h>
#include <dad/Test.h>

LUCID_GAL_BEGIN

class VertexFormat;
class VertexBuffer;
class IndexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Mesh;

LUCID_GIGL_END

struct MouseEvent;

///	RenderTest
///
///
class RenderTest : public Test
{
public:
	RenderTest() = default;

	virtual ~RenderTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	typedef LUCID_MATH::Vector<float32_t,2> Vector2;
	typedef LUCID_MATH::Vector<float32_t,3> Vector3;
	typedef LUCID_MATH::Vector<float32_t,4> Vector4;
	typedef LUCID_MATH::Matrix<float32_t,4,4> Matrix4x4;

	static int32_t const PARTICLE_MAXIMUM = 1000;
	static float64_t const EMIT_RATE;
	static float64_t const DURATION;

	struct Particle
	{
		float32_t timeEmit, timeSpan;

		Vector3 position[2];
		float32_t rotation[2];

		float32_t scale[2];

		Vector2 texcoord, texsize;
	};

	float64_t _timeStart = 0;
	int32_t _frameCount = 0;

	float64_t _emitAccum = 0;
	std::vector<Particle> _particles;

	LUCID_GIGL::Context _context;

	std::shared_ptr<LUCID_GIGL::Mesh> _mesh;
	std::shared_ptr<LUCID_GAL::VertexBuffer> _instances;

	void emitParticles(float32_t t, float32_t dt);

	void emitParticle(float32_t t, float32_t dt);

	void updateParticles(float32_t t, float32_t dt);

	LUCID_PREVENT_COPY(RenderTest);
	LUCID_PREVENT_ASSIGNMENT(RenderTest);
};

inline bool RenderTest::passed() const
{
	return true;
}
