#include "Geometry.h"
#include <lucid/gigl/Geometry.h>
#include <lucid/gigl/Resources.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GIGL {

	Geometry::Geometry(System::String ^path)
	{
		try
		{
			_internal = new std::shared_ptr<LUCID_GIGL::Geometry>(LUCID_GIGL::Resources::get<LUCID_GIGL::Geometry>(MI::marshal_as<std::string>(path)));
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
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

} /// GIGL
} /// Lucid
