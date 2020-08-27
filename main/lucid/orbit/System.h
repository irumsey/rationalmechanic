#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Simulator.h>
#include <lucid/orbit/Renderer.h>

///
///
///

namespace lucid {
namespace gigl {

	class Context;

}	///	gigl
}	/// lucid

///
///
///

namespace lucid {
namespace orbit {

	class Frame;

	///
	///
	///
	class System
	{
	public:
		System();

		virtual ~System();

		void initialize(scalar_t dayNumber /* creiteria for orbital body inclusion/exclusion */);

		void shutdown();

		scalar_t currentDayNumber() const;

		void update(scalar_t delta);

		void render(::lucid::gigl::Context const &context, float32_t time, float32_t interpolant);

	private:
		scalar_t _dayNumber[2] = { 2451544.0, 2451544.0, };

		Simulator _simulator;
		Renderer _renderer;
		Frame *_root = nullptr;

		LUCID_PREVENT_COPY(System);
		LUCID_PREVENT_ASSIGNMENT(System);
	};

	inline scalar_t System::currentDayNumber() const
	{
		return _dayNumber[1];
	}

}	///	orbit
}	///	lucid
