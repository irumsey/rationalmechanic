#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>

namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

namespace lucid {
namespace gigl {

	class Context;
	class Stage;

	///	Renderer
	///
	///
	class Renderer final
	{
	public:
		Renderer(std::string const &path);

		Renderer(::lucid::core::Reader &reader);

		~Renderer();

		Stage *stage(std::string const &name) const;

		void render(Context const &context) const;

	private:
		Stage *_stage = nullptr;

		void initialize(::lucid::core::Reader &reader);

		void shutdown();

		LUCID_PREVENT_COPY(Renderer);
		LUCID_PREVENT_ASSIGNMENT(Renderer);
	};

}	///	gigl
}	///	lucid