#include "Texture2D.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Texture2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace   MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GAL {

	Texture2D::Texture2D(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GAL::Texture2D>(LUCID_GIGL::Resources::get<LUCID_GAL::Texture2D>(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	Texture2D::~Texture2D()
	{
		this->!Texture2D();
	}

	Texture2D::!Texture2D()
	{
		_internal->reset();
		delete _internal;
	}

} /// GAL
} /// Lucid
