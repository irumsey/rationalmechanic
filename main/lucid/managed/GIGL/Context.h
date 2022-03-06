#pragma once

#include <lucid/core/Types.h>

namespace lucid {
namespace gigl	{

	class Context;

}	///	gigl
}	///	lucid

namespace Lucid {
namespace Math {

	ref class Vector2;
	ref class Vector3;
	ref class Vector4;
	ref class Quaternion;
	ref class Matrix4x4;

} /// Math
} /// Lucid

namespace Lucid {
namespace GAL {

	ref class Color;
	ref class RenderTarget2D;

} /// GAL
} /// Lucid

namespace Lucid {
namespace GIGL {

	ref class Camera2D;

	public ref class Context
	{
	public:
		Context(System::String ^path);

		~Context();

		!Context();

		void Add(System::String ^name, GAL::RenderTarget2D ^value);

		void Set(System::String ^name, float32_t value);

		void Set(System::String ^name, Math::Vector2 ^value);

		void Set(System::String ^name, Math::Vector3 ^value);

		void Set(System::String ^name, Math::Vector4 ^value);

		void Set(System::String ^name, Math::Quaternion ^value);

		void Set(System::String ^name, Math::Matrix4x4 ^value);

		void Set(System::String ^name, GAL::Color ^value);

		void Set(System::String ^name, GAL::RenderTarget2D ^value);

		void Set(System::String ^name, System::String ^query);

		property ::lucid::gigl::Context &ref { ::lucid::gigl::Context &get() { return *ptr; } }

		property ::lucid::gigl::Context *ptr { ::lucid::gigl::Context *get() { return _internal; } }

	private:
		::lucid::gigl::Context *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
