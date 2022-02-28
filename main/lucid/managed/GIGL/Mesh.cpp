#include "Mesh.h"
#include "Context.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gigl/Mesh.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>
#include <sstream>

namespace /* anonymous */
{

	namespace MI = msclr::interop;
	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace Lucid {
namespace GIGL {

	///
	///
	///

	Mesh::Mesh(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gigl::Mesh>(gigl::Resources::get<gigl::Mesh>(MI::marshal_as<std::string>(path)));

			std::ostringstream os;
			os << MI::marshal_as<std::string>(path) << " : " << (*_internal)->primitiveCount() << " primitives and " << (*_internal)->vertexCount() << " vertices";
			core::log("INFO", os.str());
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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
