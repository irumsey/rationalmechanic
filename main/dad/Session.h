#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <dad/Tests.h>

///
///
///

namespace lucid {
namespace gal {

	class RenderTarget2D;

}	///	gal
}	///	lucid

namespace lucid {
namespace gigl {

	class Mesh;

}	///	gigl
}	///	lucid

///
///
///

struct MouseEvent;
class SessionState;

///
///
///
class Session final
{
public:
	Session() = default;

	virtual ~Session() = default;

	bool passed() const;

	void setState(SessionState *state);

	void onInput(MouseEvent const &event);

	void onUpdate(float64_t t, float64_t dt);

	void onRender(float32_t t, float32_t interpolant);

private:
	friend class SessionStarting;
	friend class SessionStopping;

	friend class SessionTesting;

	SessionState *_state = nullptr;

	Tests _tests;
	Test *_test = nullptr;

	bool _passed = true;

	LUCID_PREVENT_COPY(Session);
	LUCID_PREVENT_ASSIGNMENT(Session);
};

inline bool Session::passed() const
{
	return _passed;
}
