#include "Material.h"
#include "Context.h"
#include <lucid/gigl/Material.h>
#include <lucid/gigl/Resources.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

#include <lucid/managed/GAL/Program.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GIGL {

	Material::Material(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GIGL::Material>(LUCID_GIGL::Resources::get<LUCID_GIGL::Material>(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
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
