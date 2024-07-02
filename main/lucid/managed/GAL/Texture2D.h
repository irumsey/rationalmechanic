#pragma once

#include <memory>
#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class Texture2D;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	Texture2D
	///
	///
	public ref class Texture2D
	{
	public:
		Texture2D(System::String ^path);

		~Texture2D();

		!Texture2D();

		property LUCID_GAL::Texture2D &ref { LUCID_GAL::Texture2D &get() { return *ptr; } }

		property LUCID_GAL::Texture2D *ptr { LUCID_GAL::Texture2D *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GAL::Texture2D> *_internal = nullptr;

	};

} /// GAL
} /// Lucid
