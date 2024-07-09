#pragma once

#include <lucid/core/Defines.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

class Clock;

LUCID_CORE_END

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
		LUCID_CORE::Clock *_internal = nullptr;

	};

} ///  Lucid
} /// Core