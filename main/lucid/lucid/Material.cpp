#include "Material.h"
#include "Program.h"
#include "Context.h"
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Context.h>
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Program.h>
#include <lucid/gal/Pipeline.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace lucid {

	Material::Material(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gigl::Material>(gigl::Resources::get<gigl::Material>(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	Material::~Material()
	{
		this->!Material();
	}

	Material::!Material()
	{
		_internal->reset();
		delete _internal;
	}

	void Material::begin(Context ^context)
	{
		(*_internal)->begin(context->ref);
	}

	void Material::end()
	{
		(*_internal)->end();
	}

	Program ^Material::program::get()
	{
		return gcnew Program(ref.program());
	}

}	///  lucid
