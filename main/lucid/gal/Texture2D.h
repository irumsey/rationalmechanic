#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

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
namespace gal {

	///	Texture2D
	///
	///
	class Texture2D
	{
	public:
		virtual ~Texture2D() = 0 {}

		virtual int32_t width() const = 0;

		virtual int32_t height() const = 0;

		static Texture2D *create(std::string const &path);

		static Texture2D *create(::lucid::core::Reader &reader);

	protected:
		Texture2D() {}

		LUCID_PREVENT_COPY(Texture2D);
		LUCID_PREVENT_ASSIGNMENT(Texture2D);
	};

}	///	gal
}	///	lucid
