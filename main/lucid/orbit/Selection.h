#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Defines.h>

LUCID_ORBIT_BEGIN

///	Selection
///
/// 
struct Selection
{
	/// TYPE
	/// 
	/// 
	enum TYPE
	{
		TYPE_NONE	 = 0x00,
		TYPE_STAR	 = 0x01,
		TYPE_FRAME	 = 0x02,
		TYPE_ORBIT	 = 0x03,
		TYPE_CAMERA	 = 0x04,
		TYPE_CALLOUT = 0x05,
		TYPE_OTHER	 = 0x0f,
	};

	/// SHIFT and MASK
	///
	/// Used by the renderer to render type and identification data
	/// into the selection render target.
	enum { SELECT_SHIFT = 28 };
	enum { SELECT_MASK  = 0x0fffffff};

	TYPE type = TYPE_NONE;
	uint32_t token = 0;		// selection type specific: id, index, etc
};

LUCID_ORBIT_END
