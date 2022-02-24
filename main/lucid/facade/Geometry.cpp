#include "Geometry.h"
#include <lucid/gigl/Geometry.h>
#include <lucid/gigl/Resources.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace gigl = ::lucid::gigl;

}	///	anonymous

namespace lucid
{

	Geometry::Geometry(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<gigl::Geometry>(gigl::Resources::get<gigl::Geometry>(MI::marshal_as<std::string>(path)));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	Geometry::~Geometry()
	{
		this->!Geometry();
	}

	Geometry::!Geometry()
	{
		_internal->reset();
		delete _internal;
	}

	void Geometry::draw()
	{
		(*_internal)->draw();
	}

	void Geometry::drawInstanced(int count)
	{
		(*_internal)->drawInstanced(count);
	}

}	///  lucid
