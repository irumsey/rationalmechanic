#pragma once

#include <memory>

namespace lucid {
namespace gal  {

	class Texture2D;

}	///	gal
}	///	lucid

namespace lucid {

	///	Texture2D
	///
	///
	public ref class Texture2D
	{
	public:
		Texture2D(System::String ^path);

		~Texture2D();

		!Texture2D();

		property ::lucid::gal::Texture2D &ref { ::lucid::gal::Texture2D &get() { return *ptr; } }

		property ::lucid::gal::Texture2D *ptr { ::lucid::gal::Texture2D *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gal::Texture2D> *_internal = nullptr;

	};

}	///  lucid
