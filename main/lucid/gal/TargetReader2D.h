#pragma once

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>

///
///
///
namespace lucid {
namespace gal {

	class RenderTarget2D;

}	///	gal
}	///	lucid

///
namespace lucid {
namespace gal {

	///	TargetReader2D
	///
	///
	class TargetReader2D
	{
	public:
		virtual ~TargetReader2D() = 0 {}

		virtual int32_t width() const = 0;

		virtual int32_t height() const = 0;

		virtual uint8_t const *read() const = 0;

		static TargetReader2D *create(RenderTarget2D const *target, int32_t width, int32_t height);

	protected:
		TargetReader2D() {}

		LUCID_PREVENT_COPY(TargetReader2D);
		LUCID_PREVENT_ASSIGNMENT(TargetReader2D);
	};

}	///	gal
}	///	lucid