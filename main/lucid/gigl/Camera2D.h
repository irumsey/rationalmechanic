#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Matrix.h>
#include <lucid/gigl/Types.h>

namespace lucid {
namespace core {

	class Reader;

}	///	core
}	///	lucid

namespace lucid {
namespace gigl {

	///	Camera2D
	///
	///
	class Camera2D
	{
	public:
		Camera2D();

		Camera2D(::lucid::core::Reader &reader);

		virtual ~Camera2D();

		void initOrthographic(Scalar const &width, Scalar const &height, Scalar const &znear, Scalar const &zfar);

		void initOrthographic(Scalar const &left, Scalar const &right, Scalar const &bottom, Scalar const &top, Scalar const &znear, Scalar const &zfar);

		void initPerspective(Scalar const &fov, Scalar const &aspect, Scalar const &znear, Scalar const &zfar);

		void initPerspective(Scalar const &left, Scalar const &right, Scalar const &bottom, Scalar const &top, Scalar const &znear, Scalar const &zfar);

		void look(Vector3 const &eye, Vector3 const &target, Vector3 const &up);

		Vector3 getPosition() const;

		Vector3 getForward() const;

		Vector3 getRight() const;

		Vector3 getUp() const;

		Matrix4x4 const &getViewMatrix() const;

		Matrix4x4 const &getProjMatrix() const;

		Matrix4x4 const &getViewProjMatrix() const;

	private:
		Vector3 _position;
		Matrix4x4 _viewMatrix;
		Matrix4x4 _projMatrix;
		Matrix4x4 _viewProjMatrix;

	};

	inline Vector3 Camera2D::getPosition() const
	{
		return _position;
	}

	inline Vector3 Camera2D::getForward() const
	{
		return ::lucid::math::extractViewForward(_viewMatrix);
	}

	inline Vector3 Camera2D::getRight() const
	{
		return ::lucid::math::extractViewRight(_viewMatrix);
	}

	inline Vector3 Camera2D::getUp() const
	{
		return ::lucid::math::extractViewUp(_viewMatrix);
	}

	inline Matrix4x4 const &Camera2D::getViewMatrix() const
	{
		return _viewMatrix;
	}

	inline Matrix4x4 const &Camera2D::getProjMatrix() const
	{
		return _projMatrix;
	}

	inline Matrix4x4 const &Camera2D::getViewProjMatrix() const
	{
		return _viewProjMatrix;
	}

}	///	gigl
}	///	lucid
