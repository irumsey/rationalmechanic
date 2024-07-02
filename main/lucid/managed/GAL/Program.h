#pragma once

#include <memory>
#include <lucid/core/Types.h>
#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class Program;

LUCID_GAL_END

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

		Program(std::shared_ptr<LUCID_GAL::Program> program);

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

		property LUCID_GAL::Program &ref { LUCID_GAL::Program &get() { return *ptr; } }

		property LUCID_GAL::Program *ptr { LUCID_GAL::Program *get() { return _internal->get(); } }

	private:
		std::shared_ptr<LUCID_GAL::Program> *_internal = nullptr;

	};

} /// GAL
} /// Lucid
