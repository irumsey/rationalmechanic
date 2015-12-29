#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

///
///
///
namespace lucid {
namespace gal {

	///	Unordered2D
	///
	///	Note: this is an R&D interface for experimenting with unordered access views.
	class Unordered2D
	{
	public:
		virtual ~Unordered2D() = 0 {}

		virtual int32_t width() const = 0;

		virtual int32_t height() const = 0;

		virtual void clear() = 0;

		virtual void sync() = 0;

		virtual uint32_t at(int32_t row, int32_t col) const = 0;

		static Unordered2D *create(int32_t width, int32_t height);

	protected:
		Unordered2D() {}

		LUCID_PREVENT_COPY(Unordered2D);
		LUCID_PREVENT_ASSIGNMENT(Unordered2D);
	};

}	///	gal
}	///	lucid
