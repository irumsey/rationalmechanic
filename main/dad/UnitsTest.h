#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <dad/Test.h>

///
///
///
class UnitsTest : public Test
{
public:
	UnitsTest() = default;

	virtual ~UnitsTest() = default;

	virtual bool passed() const override;

	virtual void begin(float64_t t) override;

	virtual bool update(float64_t t, float64_t dt) override;

	virtual void render(float32_t time, float32_t interpolant) override;

private:
	bool _passed = true;

	LUCID_PREVENT_COPY(UnitsTest);
	LUCID_PREVENT_ASSIGNMENT(UnitsTest);
};

inline bool UnitsTest::passed() const
{
	return _passed;
}
