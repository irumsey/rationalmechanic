#pragma once

#include <lucid/core/Defines.h>
#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

#if defined(LUCID_PROFILE_ENABLE)
#	define LUCID_PROFILER_INITIALIZE()			LUCID_CORE::Profiler::initialize()
#	define LUCID_PROFILER_ATTACH(external)		LUCID_CORE::Profiler::attach(reinterpret_cast<::lucid::core::Profiler*>(external))
#	define LUCID_PROFILER_SHUTDOWN()			LUCID_CORE::Profiler::shutdown()
#	define LUCID_PROFILE_BEGIN(name)			LUCID_CORE::Profiler::beginSample(name)
#	define LUCID_PROFILE_END()					LUCID_CORE::Profiler::endSample()
#	define LUCID_PROFILE_SCOPE(name)			LUCID_CORE::Profiler::ScopedSample scopedProfile##__LINE__(name)
#else
#	define LUCID_PROFILER_INITIALIZE()			/* empty */
#	define LUCID_PROFILER_ATTACH(external)		/* empty */
#	define LUCID_PROFILER_SHUTDOWN()			/* empty */
#	define LUCID_PROFILE_BEGIN(name)			/* empty */
#	define LUCID_PROFILE_END()					/* empty */
#	define LUCID_PROFILE_SCOPE(name)			/* empty */
#endif

LUCID_CORE_BEGIN

class Clock;

///	Profiler
///
///	Note: for speed, this stores and compares the ADDRESS of the STATIC character string passed
///	into beginProfile().  it does not compare the content of the string.
///
///	Note: attach is in lieu of initialize.  attach is used when one has one or more dll(s) and an
///	executable all statically linking core.  the pattern would be, initialize profiler in the
///	executable and attach that profiler to the profilers in the dll(s).  that way, they all use the
///	same profiler.  the dll will have to expose some method which takes the Profiler address to
///	which it can attach.
class Profiler final
{
public:
	///	Sample
	///
	///
	class Sample final
	{
	public:
		Profiler *profiler = nullptr;

		char const *name = nullptr;

		Sample *onlyParent = nullptr;
		Sample *firstChild = nullptr;
		Sample *nextSibling = nullptr;

		uint32_t count = 0;
		uint32_t recursion = 0;

		float64_t timeStart = 0.0;
		float64_t timeTotal = 0.0;

		Sample(Profiler *profiler, char const *name);

		~Sample();

		void begin();

		Sample *end();

		void addChild(Sample *child);

		Sample *findChild(char const *name);

	private:
		void addSibling(Sample *sibling);

		LUCID_PREVENT_COPY(Sample);
		LUCID_PREVENT_ASSIGNMENT(Sample);
	};

	///	ScopedSample
	///
	///
	struct ScopedSample final
	{
		ScopedSample(char const *name)
		{
			Profiler::beginSample(name);
		}

		~ScopedSample()
		{
			Profiler::endSample();
		}
	};

	~Profiler();

	static void initialize();

	static void attach(Profiler *external);

	static void shutdown();

	static void beginSample(char const *name);

	static void endSample();

	static Sample const *samples();

	static Profiler *instance();

protected:
	Profiler();

private:
	static bool attached;
	static Profiler *profiler;

	Clock *clock = nullptr;
	Sample *sample = nullptr;

	LUCID_PREVENT_COPY(Profiler);
	LUCID_PREVENT_ASSIGNMENT(Profiler);
};

inline Profiler *Profiler::instance()
{
	return profiler;
}

LUCID_CORE_END