#pragma once

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

	///	DepthTarget2D
	///
	///
	class DepthTarget2D
	{
	public:
		enum FORMAT
		{
			FORMAT_UNDEFINED = -1,

			FORMAT_UINT_D16,
			FORMAT_UINT_D24S8,
			FORMAT_FLOAT_D32,
		};

		virtual ~DepthTarget2D() = 0 {}

		virtual FORMAT format() const = 0;

		virtual int32_t width() const = 0;

		virtual int32_t height() const = 0;

		static DepthTarget2D *create(FORMAT format, int32_t width, int32_t height);

		static DepthTarget2D *create(::lucid::core::Reader &reader);

	protected:
		DepthTarget2D() {}

		LUCID_PREVENT_COPY(DepthTarget2D);
		LUCID_PREVENT_ASSIGNMENT(DepthTarget2D);
	};

}	///	gal
}	///	lucid
