#include "Context.h"
#include "Camera2D.h"
#include <lucid/gigl/Context.h>
#include <lucid/core/Logger.h>
#include <lucid/core/FileReader.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GAL/RenderTarget2D.h>
#include <lucid/managed/GAL/Types.h>
#include <lucid/managed/Math/Types.h>

namespace /* anonymous */
{
	
	namespace   MI = msclr::interop;
	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace Lucid {
namespace GIGL {

	Context::Context(System::String ^path)
	{
		try
		{
			_internal = new gigl::Context(core::FileReader(MI::marshal_as<std::string>(path)));
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
