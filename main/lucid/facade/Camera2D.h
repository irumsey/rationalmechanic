#pragma once

#include <lucid/core/Types.h>
#include <lucid/facade/Types.h>

namespace lucid {
namespace gigl {

	class Camera2D;

}	///	gigl
}	///	lucid

namespace lucid {

	public ref class Camera2D
	{
	public:
		Camera2D();

		Camera2D(System::String ^path);

		~Camera2D();

		!Camera2D();

		void InitPerspective(float fov, float aspect, float znear, float zfar);

		void InitOrthographic(float width, float height, float znear, float zfar);

		void Look(Vector3 ^position, Vector3 ^target, Vector3 ^up);

		property Vector3 ^position { Vector3 ^get(); }

		property Vector3 ^right { Vector3 ^get(); }

		property Vector3 ^forward { Vector3 ^get(); }

		property Vector3 ^up { Vector3 ^get(); }

		property Matrix4x4 ^viewMatrix { Matrix4x4 ^get(); }

		property Matrix4x4 ^projMatrix { Matrix4x4 ^get(); }

		property Matrix4x4 ^viewProjMatrix { Matrix4x4 ^get(); }

		property Matrix4x4 ^invViewProjMatrix { Matrix4x4 ^get(); }

	private:
		::lucid::gigl::Camera2D *_internal = nullptr;

	};

}
