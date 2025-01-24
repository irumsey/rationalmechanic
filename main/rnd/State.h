#pragma once

#include <rnd/Utility.h>

class Session;

///	State
///
/// 
class State abstract
{
public:
	virtual ~State() = default;

	virtual void onEnter(Session &session) const = 0;

	virtual void onLeave(Session &session) const = 0;

	virtual void onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const = 0;

	virtual void onMouseWheel(Session &session, int32_t delta) const = 0;

	virtual void onMouseMove(Session &session, point2d_t const &point) const = 0;

	virtual void update(Session &session, float64_t t, float32_t dt) const = 0;

	virtual void render(Session &session, float64_t t, float32_t interpolant) const = 0;

protected:
	State() = default;

};

///	Stopped
///
///	upon enter, this state will clean up the session
class Stopped final : public State
{
public:
	Stopped() = default;

	virtual ~Stopped() = default;

	virtual void onEnter(Session &session) const override;

	virtual void onLeave(Session &session) const override;

	virtual void onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const override;

	virtual void onMouseWheel(Session &session, int32_t delta) const override;

	virtual void onMouseMove(Session &session, point2d_t const &point) const override;

	virtual void update(Session &session, float64_t t, float32_t dt) const override;

	virtual void render(Session &session, float64_t t, float32_t interpolant) const override;

	static Stopped const *instance();

};

///	Starting
///
///	upon enter, this state will initialize the session
class Starting final : public State
{
public:
	Starting() = default;

	virtual ~Starting() = default;

	virtual void onEnter(Session &session) const override;

	virtual void onLeave(Session &session) const override;

	virtual void onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const override;

	virtual void onMouseWheel(Session &session, int32_t delta) const override;

	virtual void onMouseMove(Session &session, point2d_t const &point) const override;

	virtual void update(Session &session, float64_t t, float32_t dt) const override;

	virtual void render(Session &session, float64_t t, float32_t interpolant) const override;

	static Starting const *instance();

};

///	Simulating
///
/// upon enter, this state will begin simulating
class Simulating final : public State
{
public:
	Simulating() = default;

	virtual ~Simulating() = default;

	virtual void onEnter(Session &session) const override;

	virtual void onLeave(Session &session) const override;

	virtual void onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const override;

	virtual void onMouseWheel(Session &session, int32_t delta) const override;

	virtual void onMouseMove(Session &session, point2d_t const &point) const override;

	virtual void update(Session &session, float64_t t, float32_t dt) const override;

	virtual void render(Session &session, float64_t t, float32_t interpolant) const override;

	static Simulating const *instance();

};
