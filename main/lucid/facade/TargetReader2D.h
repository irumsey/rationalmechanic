#pragma once

namespace lucid {
namespace gal {

	class TargetReader2D;

}	///	gal
}	///	lucid

namespace lucid {

	ref class RenderTarget2D;

	public ref class TargetReader2D
	{
	public:
		TargetReader2D(RenderTarget2D ^target);

		~TargetReader2D();

		!TargetReader2D();

		unsigned int read(int x, int y);

		property ::lucid::gal::TargetReader2D &ref { ::lucid::gal::TargetReader2D &get() { return *ptr; } }

		property ::lucid::gal::TargetReader2D *ptr { ::lucid::gal::TargetReader2D *get() { return _internal; } }

	private:
		::lucid::gal::TargetReader2D *_internal = nullptr;

	};

}	///  lucid
