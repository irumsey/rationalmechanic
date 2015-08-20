#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

///
///
///

struct MouseEvent;
class Session;

///
///
///
class SessionState
{
public:
	virtual ~SessionState() = default;

	virtual void onEnter(Session *session) = 0;

	virtual void onLeave(Session *session) = 0;

	virtual void onInput(Session *session, MouseEvent const &input) = 0;

	virtual void onUpdate(Session *session, float64_t t, float64_t dt) = 0;

	virtual void onRender(Session *session, float32_t time, float32_t interpolant) = 0;

protected:
	SessionState() = default;

	LUCID_PREVENT_COPY(SessionState);
	LUCID_PREVENT_ASSIGNMENT(SessionState);
};

///
///
///
class SessionStarting : public SessionState
{
public:
	virtual ~SessionStarting() = default;

	virtual void onEnter(Session *session) override;

	virtual void onLeave(Session *session) override;

	virtual void onInput(Session *session, MouseEvent const &input) override;

	virtual void onUpdate(Session *session, float64_t t, float64_t dt) override;

	virtual void onRender(Session *session, float32_t time, float32_t interpolant) override;

	static SessionState *instance();

protected:
	SessionStarting() = default;

	LUCID_PREVENT_COPY(SessionStarting);
	LUCID_PREVENT_ASSIGNMENT(SessionStarting);
};

///
///
///
class SessionStopping : public SessionState
{
public:
	virtual ~SessionStopping() = default;

	virtual void onEnter(Session *session) override;

	virtual void onLeave(Session *session) override;

	virtual void onInput(Session *session, MouseEvent const &input) override;

	virtual void onUpdate(Session *session, float64_t t, float64_t dt) override;

	virtual void onRender(Session *session, float32_t time, float32_t interpolant) override;

	static SessionState *instance();

protected:
	SessionStopping() = default;

	LUCID_PREVENT_COPY(SessionStopping);
	LUCID_PREVENT_ASSIGNMENT(SessionStopping);
};

///
///
///
class SessionTesting : public SessionState
{
public:
	virtual ~SessionTesting() = default;

	virtual void onEnter(Session *session) override;

	virtual void onLeave(Session *session) override;

	virtual void onInput(Session *session, MouseEvent const &input) override;

	virtual void onUpdate(Session *session, float64_t t, float64_t dt) override;

	virtual void onRender(Session *session, float32_t time, float32_t interpolant) override;

	static SessionState *instance();

protected:
	SessionTesting() = default;

	LUCID_PREVENT_COPY(SessionTesting);
	LUCID_PREVENT_ASSIGNMENT(SessionTesting);
};
