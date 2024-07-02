#include "Mesh.h"
#include "Context.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>
#include <sstream>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GIGL {

	///
	///
	///

	Mesh::Mesh(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GIGL::Mesh>(LUCID_GIGL::Resources::get<LUCID_GIGL::Mesh>(MI::marshal_as<std::string>(path)));

			std::ostringstream os;
			os << MI::marshal_as<std::string>(path) << " : " << (*_internal)->primitiveCount() << " primitives and " << (*_internal)->vertexCount() << " vertices";
			LUCID_CORE::log("INFO", os.str());
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	Mesh::~Mesh()
	{
		this->!Mesh();
	}

	Mesh::!Mesh()
	{
		_internal->reset();
		delete _internal;
	}

	void Mesh::render(Context ^context)
	{
		(*_internal)->render(context->ref);
	}

} /// GIGL
} /// Lucid
