#pragma once

#include <memory>
#include <lucid/core/Types.h>

namespace lucid {
namespace gal  {

	class Program;

}	///	gal
}	///	lucid

namespace Lucid {
namespace Math {

	ref class Vector2;
	ref class Vector3;
	ref class Vector4;
	ref class Quaternion;
	ref class Matrix2x2;
	ref class Matrix3x3;
	ref class Matrix4x4;

}	/// Math
}	/// Lucid

namespace Lucid {
namespace GAL {

	ref class Parameter;
	ref class Color;
	ref class Unordered2D;
	ref class Texture2D;
	ref class RenderTarget2D;

	///	Parameter
	///
	///
	public ref class Program
	{
	public:
		Program(System::String ^path);

		Program(std::shared_ptr<::lucid::gal::Program> program);

		~Program();

		!Program();

		Parameter ^lookup(System::String ^name);

		void set(Parameter ^parameter, float32_t value);

		void set(Parameter ^parameter, Math::Vector2 ^value);

		void set(Parameter ^parameter, Math::Vector3 ^value);

		void set(Parameter ^parameter, Math::Vector4 ^value);

		void set(Parameter ^parameter, Math::Quaternion ^value);

		void set(Parameter ^parameter, Math::Matrix2x2 ^value);

		void set(Parameter ^parameter, Math::Matrix3x3 ^value);

		void set(Parameter ^parameter, Math::Matrix4x4 ^value);

		void set(Parameter ^parameter, Color ^value);

		void set(Parameter ^parameter, Unordered2D ^value);

		void set(Parameter ^parameter, Texture2D ^value);

		void set(Parameter ^parameter, RenderTarget2D ^value);

		property ::lucid::gal::Program &ref { ::lucid::gal::Program &get() { return *ptr; } }

		property ::lucid::gal::Program *ptr { ::lucid::gal::Program *get() { return _internal->get(); } }

	private:
		std::shared_ptr<::lucid::gal::Program> *_internal = nullptr;

	};

} /// GAL
} /// Lucid
