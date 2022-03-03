#pragma once

#include <lucid/core/Types.h>
#include <lucid/orbit/Renderer.h>

namespace lucid {
namespace orbit {

	///	Selection
	///
	/// 
	struct Selection
	{
		/// TYPE
		/// 
		/// Selection type which mirrors the codes used by
		/// the renderer.
		enum TYPE
		{
			TYPE_NONE  = 0,
			TYPE_STAR  = Renderer::SELECT_STAR,
			TYPE_FRAME = Renderer::SELECT_FRAME,
			TYPE_ORBIT = Renderer::SELECT_ORBIT,
			TYPE_OTHER = Renderer::SELECT_OTHER,
		};

		TYPE type = TYPE_NONE;
		uint32_t tag = 0;		// selection specific: id, index, etc
	};

}	/// orbit
}	/// lucid
