#pragma once

#include <lucid/core/Types.h>
#include <lucid/gal/Types.h>

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

		void initOrthographic(float32_t width, float32_t height, float32_t znear, float32_t zfar);

		void initOrthographic(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar);

		void initPerspective(float32_t fov, float32_t aspect, float32_t znear, float32_t zfar);

		void initPerspective(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t znear, float32_t zfar);

		void look(::lucid::gal::Vector3 const &eye, ::lucid::gal::Vector3 const &target, ::lucid::gal::Vector3 const &up);

		::lucid::gal::Vector3 getPosition() const;

		::lucid::gal::Vector3 getForward() const;

		::lucid::gal::Vector3 getRight() const;

		::lucid::gal::Vector3 getUp() const;

		::lucid::gal::Matrix4x4 const &getViewMatrix() const;

		::lucid::gal::Matrix4x4 const &getProjMatrix() const;

		::lucid::gal::Matrix4x4 const &getViewProjMatrix() const;

	private:
		::lucid::gal::Vector3 _position;
		::lucid::gal::Matrix4x4 _viewMatrix;
		::lucid::gal::Matrix4x4 _projMatrix;
		::lucid::gal::Matrix4x4 _viewProjMatrix;

	};

	inline ::lucid::gal::Vector3 Camera2D::getPosition() const
	{
		return _position;
	}

	inline ::lucid::gal::Vector3 Camera2D::getForward() const
	{
		return ::lucid::gal::Vector3(-_viewMatrix.zx, -_viewMatrix.zy, -_viewMatrix.zz);
	}

	inline ::lucid::gal::Vector3 Camera2D::getRight() const
	{
		return ::lucid::gal::Vector3(_viewMatrix.xx, _viewMatrix.xy, _viewMatrix.xz);
	}

	inline ::lucid::gal::Vector3 Camera2D::getUp() const
	{
		return ::lucid::gal::Vector3(_viewMatrix.yx, _viewMatrix.yy, _viewMatrix.yz);
	}

	inline ::lucid::gal::Matrix4x4 const &Camera2D::getViewMatrix() const
	{
		return _viewMatrix;
	}

	inline ::lucid::gal::Matrix4x4 const &Camera2D::getProjMatrix() const
	{
		return _projMatrix;
	}

	inline ::lucid::gal::Matrix4x4 const &Camera2D::getViewProjMatrix() const
	{
		return _viewProjMatrix;
	}

}	///	gigl
}	///	lucid
