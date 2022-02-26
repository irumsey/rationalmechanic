#pragma once

namespace lucid {
namespace gal  {

	class DepthTarget2D;

}	///	gal
}	///	lucid

namespace lucid {

	///	
	///
	///
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

		property ::lucid::gal::DepthTarget2D &ref { ::lucid::gal::DepthTarget2D &get() { return *ptr; } }

		property ::lucid::gal::DepthTarget2D *ptr { ::lucid::gal::DepthTarget2D *get() { return _internal; } }

	private:
		::lucid::gal::DepthTarget2D *_internal = nullptr;

	};

}	///  lucid