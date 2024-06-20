#include "Camera2D.h"
#include <lucid/math/Matrix.h>
#include <lucid/core/Reader.h>

namespace /* anonymous */
{
	
	enum TYPE
	{
		TYPE_PERSPECTIVE = 0,
		TYPE_ORTHOGRAPHIC,
	};

	template<class T> inline T read(::lucid::core::Reader &reader)
	{
		T value = T();
		reader.read(&value, sizeof(T));
		return value;
	}

}

namespace lucid {
namespace gigl {

	Camera2D::Camera2D()
	{
	}

	Camera2D::Camera2D(::lucid::core::Reader &reader)
	{
		TYPE type;
		reader.read(&type, sizeof(TYPE));

		if (TYPE_PERSPECTIVE == type)
		{
			float32_t fov = read<float32_t>(reader);
			float32_t aspect = read<float32_t>(reader);
			float32_t znear = read<float32_t>(reader);
			float32_t zfar = read<float32_t>(reader);

			initPerspective(fov, aspect, znear, zfar);
		}
		else
		{
			float32_t width = read<float32_t>(reader);
			float32_t height = read<float32_t>(reader);
			float32_t znear = read<float32_t>(reader);
			float32_t zfar = read<float32_t>(reader);

			initOrthographic(width, height, znear, zfar);
		}

		Vector3 position = read<Vector3>(reader);
		Vector3 target = read<Vector3>(reader);
		Vector3 up = read<Vector3>(reader);

		look(position, target, up);
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::initOrthographic(Scalar const &width, Scalar const &height, Scalar const &znear, Scalar const &zfar)
	{
		_projMatrix = ::lucid::math::orthographic(width, height, znear, zfar);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

	void Camera2D::initOrthographic(Scalar const &left, Scalar const &right, Scalar const &bottom, Scalar const &top, Scalar const &znear, Scalar const &zfar)
	{
		_projMatrix = ::lucid::math::orthographic(left, right, bottom, top, znear, zfar);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

	void Camera2D::initPerspective(Scalar const &fov, Scalar const &aspect, Scalar const &znear, Scalar const &zfar)
	{
		_projMatrix = ::lucid::math::perspective(fov, aspect, znear, zfar);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

	void Camera2D::initPerspective(Scalar const &left, Scalar const &right, Scalar const &bottom, Scalar const &top, Scalar const &znear, Scalar const &zfar)
	{
		_projMatrix = ::lucid::math::perspective(left, right, bottom, top, znear, zfar);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

	void Camera2D::look(Vector3 const &eye, Vector3 const &target, Vector3 const &up)
	{
		_position = eye;

		_viewMatrix = ::lucid::math::look(_position, target, up);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

}	///	gigl
}	///	lucid