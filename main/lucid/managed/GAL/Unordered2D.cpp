#include "Unordered2D.h"
#include <lucid/gal/Unordered2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace Lucid {
namespace GAL {

	Unordered2D::Unordered2D(int width, int height)
	{
		try
		{
			_internal = LUCID_GAL::Unordered2D::create(width, height);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	Unordered2D::~Unordered2D()
	{
		this->!Unordered2D();
	}

	Unordered2D::!Unordered2D()
	{
		delete _internal;
	}

} /// GAL
} /// Lucid
