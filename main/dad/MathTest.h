#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <dad/Test.h>

struct MouseEvent;

///
///
///
class MathTest : public Test
{
public:
	MathTest() = default;

	virtual ~MathTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	bool _passed = true;

	LUCID_PREVENT_COPY(MathTest);
	LUCID_PREVENT_ASSIGNMENT(MathTest);
};

inline bool MathTest::passed() const
{
	return _passed;
}

inline void MathTest::onInput(MouseEvent const &event)
{
	///	nop
}
