#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class DepthTarget2D;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	
	///
	///	Note: must match the unmanaged format
	public enum class DepthTargetFormat
	{
		UNDEFINED = -1,

		UINT_D16,
		UINT_D24S8,
		FLOAT_D32,
	};

	///	DepthTarget2D
	///
	///
	public ref class DepthTarget2D
	{
	public:
		DepthTarget2D(DepthTargetFormat format, int width, int height);

		~DepthTarget2D();

		!DepthTarget2D();

		property LUCID_GAL::DepthTarget2D &ref { LUCID_GAL::DepthTarget2D &get() { return *ptr; } }

		property LUCID_GAL::DepthTarget2D *ptr { LUCID_GAL::DepthTarget2D *get() { return _internal; } }

	private:
		LUCID_GAL::DepthTarget2D *_internal = nullptr;

	};

} /// GAL
} /// Lucid
