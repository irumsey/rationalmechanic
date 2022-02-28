#include "Texture2D.h"
#include <lucid/gigl/Resources.h>
#include <lucid/gal/Texture2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace /* anonymous */
{

	namespace   MI = msclr::interop;
	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;
	namespace gigl = ::lucid::gigl;

}

namespace Lucid {
namespace GAL {

	Texture2D::Texture2D(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gal::Texture2D>(gigl::Resources::get<gal::Texture2D>(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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
