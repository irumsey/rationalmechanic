#include "Program.h"
#include "RenderTarget2D.h"
#include "Texture2D.h"
#include "Unordered2D.h"
#include "Parameter.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Program.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/Math/Types.h>
#include <lucid/managed/GAL/Types.h>

LUCID_ANONYMOUS_BEGIN

namespace MI   = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GAL {

	Program::Program(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GAL::Program>(LUCID_GIGL::Resources::get<LUCID_GAL::Program>(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	Program::Program(std::shared_ptr<LUCID_GAL::Program> program)
	{
		_internal = new std::shared_ptr<LUCID_GAL::Program>(program);
	}

	Program::~Program()
	{
		this->!Program();
	}

	Program::!Program()
	{
		_internal->reset();
		delete _internal;
	}

	Parameter ^Program::lookup(System::String ^name)
	{
		return gcnew Parameter((*_internal)->lookup(MI::marshal_as<std::string>(name)));
	}

	void Program::set(Parameter ^parameter, float32_t value)
	{
		(*_internal)->set(parameter->ptr, value);
	}

	void Program::set(Parameter ^parameter, Math::Vector2 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Vector3 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Vector4 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Quaternion ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Matrix2x2 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Matrix3x3 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Math::Matrix4x4 ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Color ^value)
	{
		(*_internal)->set(parameter->ptr, value->ref);
	}

	void Program::set(Parameter ^parameter, Unordered2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

	void Program::set(Parameter ^parameter, Texture2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

	void Program::set(Parameter ^parameter, RenderTarget2D ^value)
	{
		(*_internal)->set(parameter->ptr, value->ptr);
	}

} /// GAL
} /// Lucid
