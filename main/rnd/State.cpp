#include "State.h"
#include "Session.h"
#include <lucid/gal/Pipeline.h>

/// 
/// 
/// 

void Stopped::onEnter(Session &session) const
{
	if (this == session.state)
		return;

	/// TBD: shutdown everything...
}

void Stopped::onLeave(Session &session) const
{
	/// NOP
}

void Stopped::onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const
{
}

void Stopped::onMouseWheel(Session &session, int32_t delta) const
{
}

void Stopped::onMouseMove(Session &session, point2d_t const &point) const
{
}

void Stopped::update(Session &session, float64_t t, float32_t dt) const
{
}

void Stopped::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(1, 0, 0, 1), 1.f, 0x00);
}

Stopped const *Stopped::instance()
{
	thread_local static Stopped _instance;
	return &_instance;
}

/// 
/// 
/// 

void Starting::onEnter(Session &session) const
{
	/// TBD: initialize...
	session.changeState(Simulating::instance());
}

void Starting::onLeave(Session &session) const
{
	/// NOP
}

void Starting::onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const
{
}

void Starting::onMouseWheel(Session &session, int32_t delta) const
{
}

void Starting::onMouseMove(Session &session, point2d_t const &point) const
{
}

void Starting::update(Session &session, float64_t t, float32_t dt) const
{
}

void Starting::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 0, 1, 1), 1.f, 0x00);
}

Starting const *Starting::instance()
{
	thread_local static Starting _instance;
	return &_instance;
}

/// 
/// 
/// 

void Simulating::onEnter(Session &session) const
{
}

void Simulating::onLeave(Session &session) const
{
}

void Simulating::onMouseButton(Session &session, MOUSE_BUTTON button, bool down, point2d_t const &point) const
{
}

void Simulating::onMouseWheel(Session &session, int32_t delta) const
{
}

void Simulating::onMouseMove(Session &session, point2d_t const &point) const
{
}

void Simulating::update(Session &session, float64_t t, float32_t dt) const
{
}

void Simulating::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1), 1.f, 0x00);
}

Simulating const *Simulating::instance()
{
	thread_local static Simulating _instance;
	return &_instance;
}
