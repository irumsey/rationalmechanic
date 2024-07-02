#pragma once

#include <lucid/gal/Defines.h>
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
			_internal = new LUCID_GAL::Color();
		}

		Color(float32_t r, float32_t g, float32_t b, float32_t a)
		{
			_internal = new LUCID_GAL::Color(r, g, b, a);
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

		property LUCID_GAL::Color const *ptr { LUCID_GAL::Color const *get() { return _internal; } }

		property LUCID_GAL::Color const &ref { LUCID_GAL::Color const &get() { return *_internal; } }

	private:
		LUCID_GAL::Color *_internal = nullptr;

	};

	///
	///
	///
	public ref class Viewport
	{
	public:
		Viewport()
		{
			_internal = new LUCID_GAL::Viewport();
		}

		Viewport(LUCID_GAL::Viewport const &viewport)
		{
			_internal = new LUCID_GAL::Viewport(viewport);
		}

		Viewport(int x, int y, int width, int height, float znear, float zfar)
		{
			_internal = new LUCID_GAL::Viewport(x, y, width, height, znear, zfar);
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

		property LUCID_GAL::Viewport const *ptr { LUCID_GAL::Viewport const *get() { return _internal; } }

		property LUCID_GAL::Viewport const &ref { LUCID_GAL::Viewport const &get() { return *_internal; } }

	private:
		LUCID_GAL::Viewport *_internal = nullptr;

	};

}	// GAL
}	// Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
