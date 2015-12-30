#include "Context.h"
#include "RenderTarget2D.h"
#include "Camera2D.h"
#include "Types.h"
#include <lucid/gigl/Context.h>
#include <lucid/core/Logger.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Reader.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{
	
	namespace core = ::lucid::core;

}	///	anonymous

namespace lucid {

	Context::Context(System::String ^path)
	{
		try
		{
			_internal = new ::lucid::gigl::Context(::lucid::core::FileReader(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	Context::~Context()
	{
		this->!Context();
	}

	Context::!Context()
	{
		delete _internal;
	}

	void Context::Add(System::String ^name, RenderTarget2D ^value)
	{
		_internal->add(MI::marshal_as<std::string>(name), value->shared);
	}

	void Context::Set(System::String ^name, float value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value;
	}

	void Context::Set(System::String ^name, Vector2 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Vector3 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Vector4 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Quaternion ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Matrix4x4 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Color ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, RenderTarget2D ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->shared;
	}

	void Context::Set(System::String ^name, System::String ^query)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = MI::marshal_as<std::string>(query);
	}

	void Context::Set(Camera2D ^camera)
	{
		_internal->lookup("viewPosition") = camera->position->ref;

		_internal->lookup("viewForward") = camera->forward->ref;
		_internal->lookup("viewRight") = camera->right->ref;
		_internal->lookup("viewUp") = camera->up->ref;

		_internal->lookup("viewMatrix") = camera->viewMatrix->ref;
		_internal->lookup("invViewMatrix") = ::lucid::math::inverse(camera->viewMatrix->ref);

		_internal->lookup("projMatrix") = camera->projMatrix->ref;
		_internal->lookup("invProjMatrix") = ::lucid::math::inverse(camera->projMatrix->ref);

		_internal->lookup("viewProjMatrix") = camera->viewProjMatrix->ref;
		_internal->lookup("invViewProjMatrix") = ::lucid::math::inverse(camera->viewProjMatrix->ref);
	}

}	///  lucid
