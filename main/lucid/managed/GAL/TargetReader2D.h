#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class TargetReader2D;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	ref class RenderTarget2D;

	public ref class TargetReader2D
	{
	public:
		TargetReader2D(RenderTarget2D ^target);

		~TargetReader2D();

		!TargetReader2D();

		unsigned int read(int x, int y);

		property LUCID_GAL::TargetReader2D &ref { LUCID_GAL::TargetReader2D &get() { return *ptr; } }

		property LUCID_GAL::TargetReader2D *ptr { LUCID_GAL::TargetReader2D *get() { return _internal; } }

	private:
		LUCID_GAL::TargetReader2D *_internal = nullptr;

	};

} /// GAL
}	///  Lucid
