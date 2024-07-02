#pragma once

#include <lucid/core/Types.h>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

class Context;

LUCID_GIGL_END

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

		property LUCID_GIGL::Context &ref { LUCID_GIGL::Context &get() { return *ptr; } }

		property LUCID_GIGL::Context *ptr { LUCID_GIGL::Context *get() { return _internal; } }

	private:
		LUCID_GIGL::Context *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
