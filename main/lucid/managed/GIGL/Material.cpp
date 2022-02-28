#include "Material.h"
#include "Context.h"
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GAL/Program.h>

namespace /* anonymous */
{

	namespace   MI = msclr::interop;
	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace Lucid {
namespace GIGL {

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

	GAL::Program ^Material::program::get()
	{
		return gcnew GAL::Program(ref.program());
	}

} /// GIGL
} /// Lucid
