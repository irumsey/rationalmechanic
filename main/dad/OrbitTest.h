#pragma once

#include <memory>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/orbit/Mechanics.h>
#include <dad/Test.h>

struct MouseEvent;

LUCID_GAL_BEGIN

class VertexBuffer;

LUCID_GAL_END

LUCID_GIGL_BEGIN

class Mesh;

LUCID_GIGL_END

///
///
///
class OrbitTest : public Test
{
public:
	OrbitTest() = default;

	virtual ~OrbitTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	bool _passed = true;

	LUCID_ORBIT::CameraFrame *_cameraFrame = nullptr;
	LUCID_ORBIT::Mechanics *_orbitalMechanics = nullptr;

	LUCID_PREVENT_COPY(OrbitTest);
	LUCID_PREVENT_ASSIGNMENT(OrbitTest);
};

inline bool OrbitTest::passed() const
{
	return _passed;
}
