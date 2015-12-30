#include "VertexFormat.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gal/VertexFormat.h>
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

	VertexFormat::VertexFormat(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gal::VertexFormat>(gigl::Resources::get<gal::VertexFormat>(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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

}	///  lucid
