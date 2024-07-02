#include "VertexFormat.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Defines.h>
#include <lucid/gal/VertexFormat.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <lucid/core/Defines.h>
#include <msclr/marshal_cppstd.h>


LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GAL {

	VertexFormat::VertexFormat(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GAL::VertexFormat>(LUCID_GIGL::Resources::get<LUCID_GAL::VertexFormat>(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	VertexFormat::~VertexFormat()
	{
		this->!VertexFormat();
	}

	VertexFormat::!VertexFormat()
	{
		delete _internal;
	}

} /// GAL
} /// Lucid
