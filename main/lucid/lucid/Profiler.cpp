#include "Profiler.h"
#include <lucid/core/Profiler.h>

namespace lucid {

	Profiler::Profiler()
	{
		LUCID_PROFILER_INITIALIZE();
	}

	Profiler::~Profiler()
	{
		this->!Profiler();
	}

	Profiler::!Profiler()
	{
		LUCID_PROFILER_SHUTDOWN();
	}

}	///	lucid
