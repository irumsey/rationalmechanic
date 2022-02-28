#pragma once

#include <lucid/gal/Types.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(name, type) property type name { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace GAL {

	///	Color
	///
	///
	public ref class Color
	{
	public:
		Color()
		{
			_internal = new ::lucid::gal::Color();
		}

		Color(float32_t r, float32_t g, float32_t b, float32_t a)
		{
			_internal = new ::lucid::gal::Color(r, g, b, a);
		}

		~Color()
		{
			this->!Color();
		}

		!Color()
		{
			delete _internal;
		}

		PROPERTY(r, float)
		PROPERTY(g, float)
		PROPERTY(b, float)
		PROPERTY(a, float)

		property ::lucid::gal::Color const *ptr { ::lucid::gal::Color const *get() { return _internal; } }

		property ::lucid::gal::Color const &ref { ::lucid::gal::Color const &get() { return *_internal; } }

	private:
		::lucid::gal::Color *_internal = nullptr;

	};

	///
	///
	///
	public ref class Viewport
	{
	public:
		Viewport()
		{
			_internal = new ::lucid::gal::Viewport();
		}

		Viewport(::lucid::gal::Viewport const &viewport)
		{
			_internal = new ::lucid::gal::Viewport(viewport);
		}

		Viewport(int x, int y, int width, int height, float znear, float zfar)
		{
			_internal = new ::lucid::gal::Viewport(x, y, width, height, znear, zfar);
		}

		~Viewport()
		{
			this->!Viewport();
		}

		!Viewport()
		{
			delete _internal;
		}

		PROPERTY(x, int)
		PROPERTY(y, int)
		PROPERTY(width, int)
		PROPERTY(height, int)
		PROPERTY(znear, float)
		PROPERTY(zfar, float)

			property ::lucid::gal::Viewport const *ptr { ::lucid::gal::Viewport const *get() { return _internal; } }

		property ::lucid::gal::Viewport const &ref { ::lucid::gal::Viewport const &get() { return *_internal; } }

	private:
		::lucid::gal::Viewport *_internal = nullptr;

	};

}	// GAL
}	// Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
