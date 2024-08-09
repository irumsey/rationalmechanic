#include "Mechanics.h"
#include "Frame.h"
#include "Selection.h"
#include <lucid/orbit/Mechanics.h>
#include <lucid/orbit/Selection.h>
#include <lucid/orbit/Utility.h>
#include <lucid/math/Algorithm.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

#include <lucid/managed/Math/Types.h>

namespace Lucid {
namespace Orbit {

	Mechanics::Mechanics(double dayNumber)
	{
		try
		{
			Initialize(dayNumber);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERR", error.what());
			Shutdown();
			throw;
		}
	}

	Mechanics::~Mechanics()
	{
		this->!Mechanics();
	}

	Mechanics::!Mechanics()
	{
		Shutdown();
	}

	void Mechanics::Initialize(double dayNumber)
	{
		Shutdown();

		_internal = new LUCID_ORBIT::Mechanics(dayNumber);
	}

	void Mechanics::Shutdown()
	{
		delete _internal;
		_internal = nullptr;
	}

	float Mechanics::DayNumber::get()
	{
		return LUCID_ORBIT::cast(_internal->dayNumber());
	}

	Frame ^Mechanics::Root::get()
	{
		return Frame::Wrap(_internal->root());
	}

	Frame ^Mechanics::default::get(size_t id)
	{
		return Frame::Wrap(_internal->frame(id));
	}

	void Mechanics::Attach(Frame ^center, Frame ^frame)
	{
		_internal->attach(center->ptr, frame->ptr);
	}

	void Mechanics::Detach(Frame ^frame)
	{
		_internal->detach(frame->ptr);
	}

	void Mechanics::Update()
	{
		try
		{
			_internal->update();
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERR", error.what());
			Shutdown();
			throw;
		}
	}

	void Mechanics::Render(CameraFrame ^cameraFrame, bool useFXAA)
	{
		try
		{
			_internal->render(cameraFrame->ptr, useFXAA);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERR", error.what());
			Shutdown();
			throw;
		}
	}

	Selection ^Mechanics::Hit(int x, int y)
	{
		return gcnew Selection(_internal->hit(x, y));
	}

}	///	Orbit
}	///	Lucid
