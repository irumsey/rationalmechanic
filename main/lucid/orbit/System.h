#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
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
	struct Selection
	{
		enum TYPE
		{
			TYPE_NONE  = 0,
			TYPE_STAR  = Renderer::SELECT_STAR,
			TYPE_FRAME = Renderer::SELECT_FRAME,
			TYPE_ORBIT = Renderer::SELECT_ORBIT,
			TYPE_OTHER = Renderer::SELECT_OTHER,
		};
		 
		TYPE type = TYPE_NONE;
		uint32_t id = 0;
		std::string description = "none";
	};

	///
	///
	///
	class System
	{
	public:
		System();

		virtual ~System();

		void initialize(scalar_t dayNumber /* add filter for frame inclusion/exclusion */);

		void shutdown();

		scalar_t currentDayNumber() const;

		Frame *create(size_t type, std::string const &name, std::string const &description);

		Frame *root() const;

		Frame *frame(size_t id) const;

		void attach(Frame *center, Frame *frame);

		void detach(Frame *frame);

		void update(scalar_t delta);

		void render(::lucid::gigl::Context const &context, float32_t time, float32_t interpolant);

		Selection hit(int32_t x, int32_t y) const;

	private:
		typedef std::unordered_map<size_t, Frame *> frame_map_t;

		scalar_t _dayNumber[2] = { constants::J2000<scalar_t>(), constants::J2000<scalar_t>(), };

		Simulator _simulator;
		Renderer _renderer;
		
		Frame *_root = nullptr;
		frame_map_t _frames;

		size_t _idNext = 1000;	///	TBD: need better id generation method
		size_t genFrameID() { return ++_idNext; }

		LUCID_PREVENT_COPY(System);
		LUCID_PREVENT_ASSIGNMENT(System);
	};

	inline scalar_t System::currentDayNumber() const
	{
		return _dayNumber[1];
	}

	inline Frame *System::root() const
	{
		return _root;
	}

	inline Frame *System::frame(size_t id) const
	{
		auto iter = _frames.find(id);
		LUCID_VALIDATE(iter != _frames.end(), "unknown frame id specified");

		return iter->second;
	}

}	///	orbit
}	///	lucid
