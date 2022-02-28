#pragma once

#include <lucid/core/Types.h>

namespace lucid {
namespace core {

	class Clock;

}	///	core
}	///	lucid

namespace Lucid {
namespace Core {

	///
	/// 
	///
	public ref class Clock
	{
	public:
		Clock();

		~Clock();

		!Clock();

		property float64_t time
		{
			float64_t get();
		};

		void reset();

	private:
		::lucid::core::Clock *_internal = nullptr;

	};

} ///  Lucid
} /// Core