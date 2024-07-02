#include "Profiler.h"
#include <lucid/core/Profiler.h>
#include <fstream>
#include <iomanip>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace   MI = msclr::interop;

size_t const    SAMPLE_COLUMN_WIDTH = 40;
size_t const     COUNT_COLUMN_WIDTH = 15;
size_t const RECURSION_COLUMN_WIDTH = 15;
size_t const      TIME_COLUMN_WIDTH = 22;
size_t const              TAB_WIDTH =  2;
size_t const          SPACING_WIDTH =  2;

void dumpProfileHeader(std::ofstream &stream)
{
	stream << std::setw(   SAMPLE_COLUMN_WIDTH +             0) <<         "Sample";
	stream << std::setw(    COUNT_COLUMN_WIDTH + SPACING_WIDTH) <<          "Count";
	stream << std::setw(RECURSION_COLUMN_WIDTH + SPACING_WIDTH) <<      "Recursion";
	stream << std::setw(     TIME_COLUMN_WIDTH + SPACING_WIDTH) << "Total Time (s)";
	stream << std::endl;

	std::string spacing(SPACING_WIDTH, ' ');

	stream << std::string(   SAMPLE_COLUMN_WIDTH, '-') << spacing;
	stream << std::string(    COUNT_COLUMN_WIDTH, '-') << spacing;
	stream << std::string(RECURSION_COLUMN_WIDTH, '-') << spacing;
	stream << std::string(     TIME_COLUMN_WIDTH, '-') << std::endl;
}

void dumpProfileSample(std::ofstream &stream, std::string const &tab, LUCID_CORE::Profiler::Sample const *sample)
{
	if (nullptr == sample)
		return;

	size_t len = tab.size() + std::string(sample->name).size();
	size_t countColumnWidth = COUNT_COLUMN_WIDTH + (SAMPLE_COLUMN_WIDTH - len);

	stream << tab;
	stream << sample->name;
	stream << std::setw(      countColumnWidth + SPACING_WIDTH) << sample->count;
	stream << std::setw(RECURSION_COLUMN_WIDTH + SPACING_WIDTH) << sample->recursion;
	stream << std::setw(     TIME_COLUMN_WIDTH + SPACING_WIDTH) << sample->timeTotal;
	stream << std::endl;

	for (LUCID_CORE::Profiler::Sample const *child = sample->firstChild; nullptr != child; child = child->nextSibling)
		dumpProfileSample(stream, tab + std::string(TAB_WIDTH, ' '), child);
}

void dumpProfileData(std::string const &path)
{
	std::ofstream stream(path);

	dumpProfileHeader(stream);

	for (LUCID_CORE::Profiler::Sample const *sample = LUCID_CORE::Profiler::samples(); nullptr != sample; sample = sample->nextSibling)
		dumpProfileSample(stream, "", sample);
}

LUCID_ANONYMOUS_END

namespace Lucid {
namespace Core {

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

	void Profiler::dump(System::String ^path)
	{
		dumpProfileData(MI::marshal_as<std::string>(path));
	}

}	/// Core
}	///	Lucid
