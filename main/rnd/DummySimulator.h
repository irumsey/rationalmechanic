#pragma once

#include <lucid/core/Types.h>

class DummySimulator
{
public:
	enum STATE
	{
		STATE_UNITIALIZED = 0,
		STATE_STOPPED,
		STATE_RUNNING,
		STATE_PAUSED
	};

	DummySimulator() = default;

	virtual ~DummySimulator() = default;

	STATE state() const
	{
		return _state;
	}

	void initialize(float64_t time)
	{
		_time = time;
		_state = STATE_STOPPED;
	}

	float64_t DummySimulator::time() const
	{
		return _time;
	}

	bool start()
	{
		if (STATE_UNITIALIZED == _state)
			return false;
		
		if (STATE_RUNNING == _state)
			return true;

		_state = STATE_RUNNING;

		// start the simulation...

		return true;
	}
	
	void update(float64_t dt)
	{
		if (STATE_RUNNING != _state)
			return;

		// update simulation for (t, dt)...

		_time += dt;
	}

	void pause()
	{
		if (STATE_RUNNING != _state)
			return;

		_state = STATE_PAUSED;

		// prob. don't need much here...
	}

private:
	STATE _state = STATE_STOPPED;
	float64_t _time = 0;

};
