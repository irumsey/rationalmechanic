#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class Unordered2D;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	Unordered2D
	///
	///
	public ref class Unordered2D
	{
	public:
		Unordered2D(int width, int height);

		~Unordered2D();

		!Unordered2D();

		property LUCID_GAL::Unordered2D &ref { LUCID_GAL::Unordered2D &get() { return *ptr; } }

		property LUCID_GAL::Unordered2D *ptr { LUCID_GAL::Unordered2D *get() { return _internal; } }

	private:
		LUCID_GAL::Unordered2D *_internal = nullptr;

	};

} /// GAL
} /// Lucid
