#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>

///
///
///
namespace lucid {
namespace core {

	class Identity;
	class Reader;

}	///	core
}	///	lucid

namespace lucid {
namespace gal {

	///
	///
	///

	class RenderState;
	class Parameter;
	class Texture2D;
	class Unordered2D;
	class RenderTarget2D;

	///	Program
	///
	///	defined as: render state, vertex shader, and pixel shader.
	///
	///	note: this is NOT a material, it will be used to implement a material
	///	at a higher level.  this is simply a thin abstraction not meant to introduce
	///	functionality beyond that of vertex and pixel shaders.
	class Program
	{
	public:
		virtual ~Program() = 0 {}

		virtual ::lucid::core::Identity const &identity() const = 0;

		virtual RenderState const *renderState() const = 0;

		virtual Parameter const *lookup(std::string const &name) const = 0;

		virtual void set(Parameter const *parameter, uint8_t const *data, int size) const = 0;

		virtual void set(Parameter const *parameter, bool value) const = 0;

		virtual void set(Parameter const *parameter, int32_t value) const = 0;

		virtual void set(Parameter const *parameter, float32_t value) const = 0;

		virtual void set(Parameter const *parameter, Color const &value) const = 0;

		virtual void set(Parameter const *parameter, Vector2 const &value) const = 0;

		virtual void set(Parameter const *parameter, Vector3 const &value) const = 0;

		virtual void set(Parameter const *parameter, Vector4 const &value) const = 0;

		virtual void set(Parameter const *parameter, Quaternion const &value) const = 0;

		virtual void set(Parameter const *parameter, Matrix2x2 const &value) const = 0;

		virtual void set(Parameter const *parameter, Matrix3x3 const &value) const = 0;

		virtual void set(Parameter const *parameter, Matrix4x4 const &value) const = 0;

		virtual void set(Parameter const *parameter, Texture2D const *value) const = 0;

		virtual void set(Parameter const *parameter, Unordered2D const *value) const = 0;

		virtual void set(Parameter const *parameter, RenderTarget2D const *value) const = 0;

		static Program *create(std::string const &path);

		static Program *create(::lucid::core::Reader &reader);

	protected:
		Program() {}

		LUCID_PREVENT_COPY(Program);
		LUCID_PREVENT_ASSIGNMENT(Program);
	};

}	///	gal
}	///	lucid
