#pragma once

namespace lucid {
namespace gal  {

	class Unordered2D;

}	///	gal
}	///	lucid

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

		property ::lucid::gal::Unordered2D &ref { ::lucid::gal::Unordered2D &get() { return *ptr; } }

		property ::lucid::gal::Unordered2D *ptr { ::lucid::gal::Unordered2D *get() { return _internal; } }

	private:
		::lucid::gal::Unordered2D *_internal = nullptr;

	};

} /// GAL
} /// Lucid
