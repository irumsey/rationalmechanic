#pragma once

#include <lucid/core/Types.h>

namespace lucid {
namespace core	{

}	///	core
}	///	lucid

namespace lucid {
namespace gigl	{

	class Context;

}	///	gigl
}	///	lucid

namespace lucid {

	ref class Vector2;
	ref class Vector3;
	ref class Vector4;
	ref class Quaternion;
	ref class Matrix4x4;
	ref class Color;
	ref class Camera2D;
	ref class RenderTarget2D;

	public ref class Context
	{
	public:
		Context(System::String ^path);

		~Context();

		!Context();
		
		void Add(System::String ^name, RenderTarget2D ^value);

		void Set(System::String ^name, float value);

		void Set(System::String ^name, Vector2 ^value);

		void Set(System::String ^name, Vector3 ^value);

		void Set(System::String ^name, Vector4 ^value);

		void Set(System::String ^name, Quaternion ^value);

		void Set(System::String ^name, Matrix4x4 ^value);

		void Set(System::String ^name, Color ^value);

		void Set(System::String ^name, RenderTarget2D ^value);

		void Set(System::String ^name, System::String ^query);

		void Set(Camera2D ^camera);

		property ::lucid::gigl::Context &ref { ::lucid::gigl::Context &get() { return *ptr; } }

		property ::lucid::gigl::Context *ptr { ::lucid::gigl::Context *get() { return _internal; } }

	private:
		::lucid::gigl::Context *_internal = nullptr;

	};

}	///  lucid
