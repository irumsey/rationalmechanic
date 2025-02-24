#include "Context.h"
#include "Camera2D.h"
#include <lucid/gigl/Context.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Unserializer.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GAL/RenderTarget2D.h>
#include <lucid/managed/GAL/Types.h>
#include <lucid/managed/Math/Types.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GIGL {

	Context::Context(System::String ^path)
	{
		try
		{
			_internal = new LUCID_GIGL::Context(LUCID_CORE::Unserializer(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
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

	void Context::Add(System::String ^name, GAL::RenderTarget2D ^value)
	{
		_internal->add(MI::marshal_as<std::string>(name), value->shared);
	}

	void Context::Set(System::String ^name, float32_t value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value;
	}

	void Context::Set(System::String ^name, Math::Vector2 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Math::Vector3 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Math::Vector4 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Math::Quaternion ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, Math::Matrix4x4 ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, GAL::Color ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->ref;
	}

	void Context::Set(System::String ^name, GAL::RenderTarget2D ^value)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = value->shared;
	}

	void Context::Set(System::String ^name, System::String ^query)
	{
		_internal->lookup(MI::marshal_as<std::string>(name)) = MI::marshal_as<std::string>(query);
	}

} /// GIGL
} /// Lucid
