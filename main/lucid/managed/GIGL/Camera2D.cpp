#include "Camera2D.h"
#include <lucid/math/Matrix.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/Math/Types.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GIGL {

	Camera2D::Camera2D(System::String ^path)
	{
		try
		{
			_internal = new LUCID_GIGL::Camera2D(LUCID_CORE::FileReader(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	Camera2D::Camera2D(LUCID_GIGL::Camera2D const &camera)
	{
		_internal = new LUCID_GIGL::Camera2D(camera);
	}

	Camera2D::Camera2D()
	{
		_internal = new LUCID_GIGL::Camera2D();
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

	void Camera2D::Look(Math::Vector3 ^position, Math::Vector3 ^target, Math::Vector3 ^up)
	{
		_internal->look(position->ref, target->ref, up->ref);
	}

	Math::Vector3 ^Camera2D::position::get()
	{
		return gcnew Math::Vector3(_internal->getPosition());
	}

	Math::Vector3 ^Camera2D::right::get()
	{
		return gcnew Math::Vector3(_internal->getRight());
	}

	Math::Vector3 ^Camera2D::forward::get()
	{
		return gcnew Math::Vector3(_internal->getForward());
	}

	Math::Vector3 ^Camera2D::up::get()
	{
		return gcnew Math::Vector3(_internal->getUp());
	}

	Math::Matrix4x4 ^Camera2D::viewMatrix::get()
	{
		return gcnew Math::Matrix4x4(_internal->getViewMatrix());
	}

	Math::Matrix4x4 ^Camera2D::projMatrix::get()
	{
		return gcnew Math::Matrix4x4(_internal->getProjMatrix());
	}

	Math::Matrix4x4 ^Camera2D::viewProjMatrix::get()
	{
		return gcnew Math::Matrix4x4(_internal->getViewProjMatrix());
	}

	Math::Matrix4x4 ^Camera2D::invViewProjMatrix::get()
	{
		return gcnew Math::Matrix4x4(LUCID_MATH::inverse(_internal->getViewProjMatrix()));
	}

} /// GIGL
} /// Lucid
