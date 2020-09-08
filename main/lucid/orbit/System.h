#pragma once

#include <unordered_map>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/orbit/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/orbit/Types.h>
#include <lucid/orbit/Constants.h>
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

		Frame const *root() const;

		Frame const *frame(size_t id) const;

		void update(scalar_t delta);

		void render(::lucid::gigl::Context const &context, float32_t time, float32_t interpolant);

	private:
		typedef std::unordered_map<size_t, Frame *> frame_map_t;

		scalar_t _dayNumber[2] = { constants::J2000<scalar_t>(), constants::J2000<scalar_t>(), };

		Simulator _simulator;
		Renderer _renderer;
		
		Frame *_root = nullptr;
		frame_map_t _frames;

		LUCID_PREVENT_COPY(System);
		LUCID_PREVENT_ASSIGNMENT(System);
	};

	inline scalar_t System::currentDayNumber() const
	{
		return _dayNumber[1];
	}

	inline Frame const *System::root() const
	{
		return _root;
	}

	inline Frame const *System::frame(size_t id) const
	{
		auto iter = _frames.find(id);
		LUCID_VALIDATE(iter != _frames.end(), "unknown frame id specified");

		return iter->second;
	}

}	///	orbit
}	///	lucid
