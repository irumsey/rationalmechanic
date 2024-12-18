#include "SessionState.h"
#include "Session.h"
#include "Tests.h"
#include "Test.h"
#include "UserInput.h"
#include "Utility.h"
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Profiler.h>
#include <Windows.h>
#include <cassert>

///
///
///

void SessionStarting::onEnter(Session *session)
{
	LUCID_CORE::log("INFO", "starting...");
	LUCID_PROFILER_INITIALIZE();

	///	TBD: initialization code...
}

void SessionStarting::onLeave(Session *session)
{
}

void SessionStarting::onInput(Session *session, MouseEvent const &event)
{
}

void SessionStarting::onUpdate(Session *session, float64_t t, float64_t dt)
{
	session->setState(SessionTesting::instance());
}

void SessionStarting::onRender(Session *session, float32_t time, float32_t interpolant)
{
	LUCID_GAL_PIPELINE.clear(true, true, true, ::lucid::gal::Color(0, 1, 0, 1));;
}

SessionState *SessionStarting::instance()
{
	static SessionStarting theInstance;
	return &theInstance;
}

///
///
///

void SessionStopping::onEnter(Session *session)
{
	///	TBD: clean up code here...

	session->setState(nullptr);
}

void SessionStopping::onLeave(Session *session)
{
	LUCID_CORE::log("INFO", "stopped.");
	LUCID_PROFILER_SHUTDOWN();
}

void SessionStopping::onInput(Session *session, MouseEvent const &event)
{
	///	NOP
}

void SessionStopping::onUpdate(Session *session, float64_t t, float64_t dt)
{
	///	NOP
}

void SessionStopping::onRender(Session *session, float32_t time, float32_t interpolant)
{
	///	NOP
}

SessionState *SessionStopping::instance()
{
	static SessionStopping theInstance;
	return &theInstance;
}

///
///
///

void SessionTesting::onEnter(Session *session)
{
	LUCID_CORE::log("INFO", "beginning tests...");
	session->_passed = true;
}

void SessionTesting::onLeave(Session *session)
{
	///	if there is a test in process...
	delete session->_test;
	session->_test = nullptr;

	LUCID_CORE::log("INFO", "testing complete.");
}

void SessionTesting::onInput(Session *session, MouseEvent const &event)
{
	///	input may happen before complete initialization...
	if (nullptr == session->_test)
		return;
	session->_test->onInput(event);
}

void SessionTesting::onUpdate(Session *session, float64_t t, float64_t dt)
{
	if (nullptr == session->_test)
	{
		Tests &tests = session->_tests;
		session->_test = tests.nextTest();

		if (nullptr != session->_test)
		{
			session->_test->begin(t);
		}
		else
		{
			::PostQuitMessage(0);
			return;
		}
	}

	if (session->_test->update(t, dt))
	{
		delete session->_test;
		session->_test = nullptr;
	}
}

void SessionTesting::onRender(Session *session, float32_t time, float32_t interpolant)
{
	if (nullptr != session->_test)
	{
		session->_test->render(time, interpolant);
	}
}

SessionState *SessionTesting::instance()
{
	static SessionTesting theInstance;
	return &theInstance;
}
