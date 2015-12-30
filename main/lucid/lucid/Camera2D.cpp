#include "Camera2D.h"
#include <lucid/gigl/Camera2D.h>
#include <lucid/math/Matrix.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace math = ::lucid::math;

}	///	anonymous

namespace lucid
{

	Camera2D::Camera2D()
	{
		_internal = new ::lucid::gigl::Camera2D();
	}

	Camera2D::Camera2D(System::String ^path)
	{
		try
		{
			_internal = new ::lucid::gigl::Camera2D(::lucid::core::FileReader(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	Camera2D::~Camera2D()
	{
		this->!Camera2D();
	}

	Camera2D::!Camera2D()
	{
		delete _internal;
	}

	void Camera2D::InitPerspective(float fov, float aspect, float znear, float zfar)
	{
		_internal->initPerspective(fov, aspect, znear, zfar);
	}

	void Camera2D::InitOrthographic(float width, float height, float znear, float zfar)
	{
		_internal->initOrthographic(width, height, znear, zfar);
	}

	void Camera2D::Look(Vector3 ^position, Vector3 ^target, Vector3 ^up)
	{
		_internal->look(position->ref, target->ref, up->ref);
	}

	Vector3 ^Camera2D::position::get()
	{
		return gcnew Vector3(_internal->getPosition());
	}

	Vector3 ^Camera2D::right::get()
	{
		return gcnew Vector3(_internal->getRight());
	}

	Vector3 ^Camera2D::forward::get()
	{
		return gcnew Vector3(_internal->getForward());
	}

	Vector3 ^Camera2D::up::get()
	{
		return gcnew Vector3(_internal->getUp());
	}

	Matrix4x4 ^Camera2D::viewMatrix::get()
	{
		return gcnew Matrix4x4(_internal->getViewMatrix());
	}

	Matrix4x4 ^Camera2D::projMatrix::get()
	{
		return gcnew Matrix4x4(_internal->getProjMatrix());
	}

	Matrix4x4 ^Camera2D::viewProjMatrix::get()
	{
		return gcnew Matrix4x4(_internal->getViewProjMatrix());
	}

	Matrix4x4 ^Camera2D::invViewProjMatrix::get()
	{
		return gcnew Matrix4x4(math::inverse(_internal->getViewProjMatrix()));
	}

}	///  lucid
