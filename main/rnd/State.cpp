#include "State.h"
#include "Session.h"
#include <lucid/gui/Frame.h>
#include <lucid/gui/Events.h>
#include <lucid/gal/Pipeline.h>

/// 
/// 
/// 

void Stopped::onEnter(Session &session) const
{
}

void Stopped::onLeave(Session &session) const
{
}

void Stopped::onEvent(Session &session, LUCID_GUI::MouseEvent const &event) const
{
}

void Stopped::update(Session &session, float64_t t, float32_t dt) const
{
}

void Stopped::render(Session &session, float64_t t, float32_t interpolant) const
{
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
	session.changeState(Configuring::instance());
}

void Starting::onLeave(Session &session) const
{
	/// NOP
}

void Starting::onEvent(Session &session, LUCID_GUI::MouseEvent const &event) const
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

void Stopping::onEnter(Session &session) const
{
	/// TBD: shutdown everything...
	session.changeState(Stopped::instance());
}

void Stopping::onLeave(Session &session) const
{
	/// NOP
}

void Stopping::onEvent(Session &session, LUCID_GUI::MouseEvent const &event) const
{
}

void Stopping::update(Session &session, float64_t t, float32_t dt) const
{
}

void Stopping::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(1, 0, 0, 1), 1.f, 0x00);
}

Stopping const *Stopping::instance()
{
	thread_local static Stopping _instance;
	return &_instance;
}

/// 
/// 
/// 

void Configuring::onEnter(Session &session) const
{
}

void Configuring::onLeave(Session &session) const
{
}

void Configuring::onEvent(Session &session, LUCID_GUI::MouseEvent const &event) const
{
}

void Configuring::update(Session &session, float64_t t, float32_t dt) const
{
}

void Configuring::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 1, 0, 1), 1.f, 0x00);
}

Configuring const *Configuring::instance()
{
	thread_local static Configuring _instance;
	return &_instance;
}

/// 
/// 
/// 

void Running::onEnter(Session &session) const
{
}

void Running::onLeave(Session &session) const
{
}

void Running::onEvent(Session &session, LUCID_GUI::MouseEvent const &event) const
{
}

void Running::update(Session &session, float64_t t, float32_t dt) const
{
}

void Running::render(Session &session, float64_t t, float32_t interpolant) const
{
	LUCID_GAL_PIPELINE.clear(true, true, true, LUCID_GAL::Color(0, 0, 0, 1), 1.f, 0x00);
}

Running const *Running::instance()
{
	thread_local static Running _instance;
	return &_instance;
}
