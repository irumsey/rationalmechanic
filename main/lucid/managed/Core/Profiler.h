#pragma once

namespace Lucid {
namespace Core {

	public ref class Profiler
	{
	public:
		Profiler();

		~Profiler();

		!Profiler();

		void dump(System::String ^path);

	};

}	///	Core
}	///	Lucid
