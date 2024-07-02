#pragma once

#include <string>
#include <lucid/core/Profiler.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Constants.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Algorithm.h>

LUCID_ORBIT_BEGIN

class Frame;

///
///
///
class Simulator : public Algorithm
{
public:
	Simulator();

	virtual ~Simulator();

	virtual void evaluate(DynamicPoint *point) override;

	virtual void evaluate(OrbitalBody *body) override;

	virtual void evaluate(DynamicBody *body) override;

	virtual void evaluate(CameraFrame *camera) override;

	void initialize();

	void shutdown();

	void simulate(Frame *root, scalar_t dayNumber, scalar_t delta);

private:
	scalar_t _dayNumber = constants::J2000<float64_t>;
	scalar_t _delta = 0.0;

	void simulate(Frame *frame);

	void basicEvaluation(Frame *frame);

	LUCID_PREVENT_COPY(Simulator);
	LUCID_PREVENT_ASSIGNMENT(Simulator);
};

inline void Simulator::simulate(Frame *root, scalar_t dayNumber, scalar_t delta)
{
	LUCID_PROFILE_SCOPE("Simulator::simulate(...)");

	_dayNumber = dayNumber;
	_delta = delta;

	simulate(root);
}

LUCID_ORBIT_END