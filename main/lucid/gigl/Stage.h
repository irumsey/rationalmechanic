#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>

namespace lucid {
namespace gigl {

	class Context;

	///	Stage
	///
	///
	class Stage
	{
	public:
		virtual ~Stage() = default;

		virtual Stage const *next() const = 0;

		virtual Stage *next() = 0;

		virtual std::string const &name() const = 0;

		virtual void render(Context const &context) const = 0;

	protected:
		Stage() = default;

		LUCID_PREVENT_COPY(Stage);
		LUCID_PREVENT_ASSIGNMENT(Stage);
	};

}	///	gigl
}	///	lucid