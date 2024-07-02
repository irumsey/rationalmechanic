#pragma once

#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Defines.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GAL_BEGIN

///	RenderState
///
///	opaque class representing render state.
class RenderState
{
public:
	virtual ~RenderState() = 0 {}

	static RenderState *create(LUCID_CORE::Reader &reader);

protected:
	RenderState() {}

	LUCID_PREVENT_COPY(RenderState);
	LUCID_PREVENT_ASSIGNMENT(RenderState);
};

LUCID_GAL_END