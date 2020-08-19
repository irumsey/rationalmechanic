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

///
///
///
namespace lucid {
namespace gal {

	class VertexFormat;
	class VertexBuffer;
	class IndexBuffer;

}	///	gal
}	///	lucid

///
///
///
namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

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

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	typedef lucid::math::Vector<float32_t,2> Vector2;
	typedef lucid::math::Vector<float32_t,3> Vector3;
	typedef lucid::math::Vector<float32_t,4> Vector4;
	typedef lucid::math::Matrix<float32_t,4,4> Matrix4x4;

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

	lucid::gigl::Context _context;

	std::shared_ptr<lucid::gigl::Mesh> _mesh;
	std::shared_ptr<lucid::gal::VertexBuffer> _instances;

	std::shared_ptr<lucid::gigl::Mesh> _ring;

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
