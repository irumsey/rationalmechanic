#pragma once

#include <memory>
#include <vector>
#include <lucid/core/Error.h>
#include <lucid/gal/Types.h>

///
///
///

namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

///
///
///

namespace lucid {
namespace gigl {

	class Model;

}	///	orbit
}	///	lucid

///
///
///

namespace lucid {
namespace orbit {

	///
	///
	///
	struct DetailLevels
	{
		static size_t const INVALID_LEVEL = -1;

		///
		///
		///
		struct Level
		{
			std::shared_ptr<::lucid::gigl::Model> model;

			float32_t range[2] = { 0.f, 0.f, };

			::lucid::gal::Color color;
			float32_t scale = 1.f;
			::lucid::gal::Vector4 parameters;
		};

		std::vector<Level> levels;
		size_t index = INVALID_LEVEL;

		DetailLevels() = default;

		DetailLevels(::lucid::core::Reader &reader);

		~DetailLevels() = default;

		Level const &operator[](size_t index) const
		{
			LUCID_VALIDATE(index < levels.size(), "invalid detail level index");
			return levels[index];
		}

		size_t level(float32_t distance);

		void read(::lucid::core::Reader &reader);
	};

}	///	orbit
}	///	lucid
