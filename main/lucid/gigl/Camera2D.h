#pragma once

#include <lucid/core/Types.h>
#include <lucid/math/Matrix.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gigl/Types.h>

LUCID_CORE_BEGIN

class Reader;

LUCID_CORE_END

LUCID_GIGL_BEGIN

///	Camera2D
///
///
class Camera2D
{
public:
	Camera2D();

	Camera2D(LUCID_CORE::Reader &reader);

	virtual ~Camera2D();

	void initOrthographic(float32_t width, float32_t height, float32_t znear, float32_t zfar);

	void initOrthographic(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar);

	void initPerspective(float32_t fov, float32_t aspect, float32_t znear, float32_t zfar);

	void initPerspective(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar);

	void look(LUCID_GAL::Vector3 const &eye, LUCID_GAL::Vector3 const &target, LUCID_GAL::Vector3 const &up);

	LUCID_GAL::Vector3 getPosition() const;

	LUCID_GAL::Vector3 getForward() const;

	LUCID_GAL::Vector3 getRight() const;

	LUCID_GAL::Vector3 getUp() const;

	LUCID_GAL::Matrix4x4 const &getViewMatrix() const;

	LUCID_GAL::Matrix4x4 const &getProjMatrix() const;

	LUCID_GAL::Matrix4x4 const &getViewProjMatrix() const;

private:
	LUCID_GAL::Vector3 _position;
	LUCID_GAL::Matrix4x4 _viewMatrix;
	LUCID_GAL::Matrix4x4 _projMatrix;
	LUCID_GAL::Matrix4x4 _viewProjMatrix;

};

inline LUCID_GAL::Vector3 Camera2D::getPosition() const
{
	return _position;
}

inline LUCID_GAL::Vector3 Camera2D::getForward() const
{
	return LUCID_MATH::extractViewForward(_viewMatrix);
}

inline LUCID_GAL::Vector3 Camera2D::getRight() const
{
	return LUCID_MATH::extractViewRight(_viewMatrix);
}

inline LUCID_GAL::Vector3 Camera2D::getUp() const
{
	return LUCID_MATH::extractViewUp(_viewMatrix);
}

inline LUCID_GAL::Matrix4x4 const &Camera2D::getViewMatrix() const
{
	return _viewMatrix;
}

inline LUCID_GAL::Matrix4x4 const &Camera2D::getProjMatrix() const
{
	return _projMatrix;
}

inline LUCID_GAL::Matrix4x4 const &Camera2D::getViewProjMatrix() const
{
	return _viewProjMatrix;
}

LUCID_GIGL_END