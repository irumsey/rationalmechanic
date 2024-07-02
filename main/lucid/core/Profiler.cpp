#include "Profiler.h"
#include "Clock.h"

LUCID_CORE_BEGIN

Profiler::Sample::Sample(Profiler *profiler, char const *name)
	: profiler(profiler)
	, name(name)
{
}

Profiler::Sample::~Sample()
{
	delete firstChild;
	delete nextSibling;
}

void Profiler::Sample::begin()
{
	if (0 == recursion)
	{
		timeStart = profiler->clock->time();
	}
	++recursion;
	++count;
}

Profiler::Sample *Profiler::Sample::end()
{
	--recursion;
	if (0 == recursion)
	{
		timeTotal += profiler->clock->time() - timeStart;
		return onlyParent;
	}
	return this;
}

void Profiler::Sample::addChild(Sample *child)
{
	child->onlyParent = this;
	if (nullptr == firstChild)
		firstChild = child;
	else
		firstChild->addSibling(child);
}

Profiler::Sample *Profiler::Sample::findChild(char const *name)
{
	///	linear search, each sample will likely have just a few child samples.
	///	if this becomes an issue use hash map...
	for (Sample *child = firstChild; child; child = child->nextSibling)
		if (name == child->name)
			return child;

	Sample *child = new Sample(profiler, name);
	addChild(child);

	return child;
}

void Profiler::Sample::addSibling(Sample *sibling)
{
	if (nullptr == nextSibling)
		nextSibling = sibling;
	else
		nextSibling->addSibling(sibling);
}

///
///
///

bool Profiler::attached = false;
Profiler *Profiler::profiler = nullptr;

Profiler::Profiler()
{
	clock = Clock::create();
	sample = new Sample(this, "root");
}

Profiler::~Profiler()
{
	delete sample;
	delete clock;
}

void Profiler::initialize()
{
	attached = false;
	profiler = new Profiler();
}

void Profiler::attach(Profiler *external)
{
	attached = true;
	profiler = external;
}

void Profiler::shutdown()
{
	if (!attached) delete profiler;
	profiler = nullptr;
	attached = false;
}

void Profiler::beginSample(char const *name)
{
	if (name != profiler->sample->name)
	{
		profiler->sample = profiler->sample->findChild(name);
	}
	profiler->sample->begin();
}

void Profiler::endSample()
{
	profiler->sample = profiler->sample->end();
}
	
Profiler::Sample const *Profiler::samples()
{
	if (nullptr == profiler)
		return nullptr;
	return profiler->sample->firstChild;
}

LUCID_CORE_END