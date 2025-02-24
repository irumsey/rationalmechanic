#include "Camera2D.h"
#include <lucid/math/Matrix.h>
#include <lucid/core/Unserializer.h>

LUCID_ANONYMOUS_BEGIN

enum TYPE
{
	TYPE_PERSPECTIVE = 0,
	TYPE_ORTHOGRAPHIC,
};

LUCID_ANONYMOUS_END

LUCID_GIGL_BEGIN

Camera2D::Camera2D()
{
}

Camera2D::Camera2D(LUCID_CORE::Unserializer &reader)
{
	TYPE type = reader.read<TYPE>();

	if (TYPE_PERSPECTIVE == type)
	{
		float32_t fov = reader.read<float32_t>();
		float32_t aspect = reader.read<float32_t>();
		float32_t znear = reader.read<float32_t>();
		float32_t zfar = reader.read<float32_t>();

		initPerspective(fov, aspect, znear, zfar);
	}
	else
	{
		float32_t width = reader.read<float32_t>();
		float32_t height = reader.read<float32_t>();
		float32_t znear = reader.read<float32_t>();
		float32_t zfar = reader.read<float32_t>();

		initOrthographic(width, height, znear, zfar);
	}

	LUCID_GAL::Vector3 position = reader.read<LUCID_GAL::Vector3>();
	LUCID_GAL::Vector3 target = reader.read<LUCID_GAL::Vector3>();
	LUCID_GAL::Vector3 up = reader.read<LUCID_GAL::Vector3>();

	look(position, target, up);
}

Camera2D::~Camera2D()
{
}

void Camera2D::initOrthographic(float32_t width, float32_t height, float32_t znear, float32_t zfar)
{
	_projMatrix = LUCID_MATH::orthographic(width, height, znear, zfar);
	_viewProjMatrix = _projMatrix * _viewMatrix;
}

void Camera2D::initOrthographic(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar)
{
	_projMatrix = LUCID_MATH::orthographic(left, right, bottom, top, znear, zfar);
	_viewProjMatrix = _projMatrix * _viewMatrix;
}

void Camera2D::initPerspective(float32_t fov, float32_t aspect, float32_t znear, float32_t zfar)
{
	_projMatrix = LUCID_MATH::perspective(fov, aspect, znear, zfar);
	_viewProjMatrix = _projMatrix * _viewMatrix;
}

void Camera2D::initPerspective(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar)
{
	_projMatrix = LUCID_MATH::perspective(left, right, bottom, top, znear, zfar);
	_viewProjMatrix = _projMatrix * _viewMatrix;
}

void Camera2D::look(LUCID_GAL::Vector3 const &eye, LUCID_GAL::Vector3 const &target, LUCID_GAL::Vector3 const &up)
{
	_position = eye;

	_viewMatrix = LUCID_MATH::look(_position, target, up);
	_viewProjMatrix = _projMatrix * _viewMatrix;
}

LUCID_GIGL_END