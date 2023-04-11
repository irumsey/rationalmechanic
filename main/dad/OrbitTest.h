#pragma once

#include <memory>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/orbit/Mechanics.h>
#include <dad/Test.h>

struct MouseEvent;

namespace lucid {
namespace gal {

	class VertexBuffer;

}	///	gal
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

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

	lucid::orbit::CameraFrame *_cameraFrame = nullptr;
	lucid::orbit::Mechanics *_orbitalMechanics = nullptr;

	LUCID_PREVENT_COPY(OrbitTest);
	LUCID_PREVENT_ASSIGNMENT(OrbitTest);
};

inline bool OrbitTest::passed() const
{
	return _passed;
}
