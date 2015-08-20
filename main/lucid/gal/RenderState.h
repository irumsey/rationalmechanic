#pragma once

#include <lucid/core/Noncopyable.h>

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

	///	RenderState
	///
	///	opaque class representing render state.
	class RenderState
	{
	public:
		virtual ~RenderState() = 0 {}

		static RenderState *create(::lucid::core::Reader &reader);

	protected:
		RenderState() {}

		LUCID_PREVENT_COPY(RenderState);
		LUCID_PREVENT_ASSIGNMENT(RenderState);
	};

}	///	gal
}	///	lucid