#include "Camera2D.h"
#include <lucid/math/Matrix.h>
#include <lucid/core/Reader.h>

LUCID_ANONYMOUS_BEGIN

enum TYPE
{
	TYPE_PERSPECTIVE = 0,
	TYPE_ORTHOGRAPHIC,
};

template<class T> inline T read(LUCID_CORE::Reader &reader)
{
	T value = T();
	reader.read(&value, sizeof(T));
	return value;
}

LUCID_ANONYMOUS_END

LUCID_GIGL_BEGIN

Camera2D::Camera2D()
{
}

Camera2D::Camera2D(LUCID_CORE::Reader &reader)
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

	LUCID_GAL::Vector3 position = read<LUCID_GAL::Vector3>(reader);
	LUCID_GAL::Vector3 target = read<LUCID_GAL::Vector3>(reader);
	LUCID_GAL::Vector3 up = read<LUCID_GAL::Vector3>(reader);

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