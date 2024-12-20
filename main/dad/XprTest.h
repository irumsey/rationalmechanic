#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <dad/Test.h>

struct MouseEvent;

///
///
///
class XprTest : public Test
{
public:
	XprTest() = default;

	virtual ~XprTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual void onInput(MouseEvent const &event) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	bool _passed = true;

	LUCID_PREVENT_COPY(XprTest);
	LUCID_PREVENT_ASSIGNMENT(XprTest);
};

inline bool XprTest::passed() const
{
	return _passed;
}

inline void XprTest::onInput(MouseEvent const &event)
{
	///	nop
}
