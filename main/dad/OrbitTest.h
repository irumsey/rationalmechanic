#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Types.h>
#include <lucid/gigl/Context.h>
#include <lucid/orbit/System.h>
#include <dad/Test.h>

///
///
///
namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

struct MouseEvent;

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

	::lucid::gigl::Context _context;

	::lucid::gal::Vector3 _viewPosition;
	::lucid::gal::Vector3 _viewDirection;

	lucid::orbit::System _orbitalSystem;

	LUCID_PREVENT_COPY(OrbitTest);
	LUCID_PREVENT_ASSIGNMENT(OrbitTest);
};

inline bool OrbitTest::passed() const
{
	return _passed;
}
