#pragma once

namespace lucid {

	public ref class Profiler
	{
	public:
		Profiler();

		~Profiler();

		!Profiler();

		void dump(System::String ^path);

	};

}	///	lucid