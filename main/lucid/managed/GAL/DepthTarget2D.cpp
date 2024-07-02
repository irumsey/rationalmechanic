#include "DepthTarget2D.h"
#include <lucid/gal/DepthTarget2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace Lucid {
namespace GAL {


	DepthTarget2D::DepthTarget2D(DepthTargetFormat format, int width, int height)
	{
		try
		{
			_internal = LUCID_GAL::DepthTarget2D::create(static_cast<LUCID_GAL::DepthTarget2D::FORMAT>(format), width, height);
		}
		catch (LUCID_CORE::Error const &error)
		{
			LUCID_CORE::log("ERROR", error.what());
			throw;
		}
	}

	DepthTarget2D::~DepthTarget2D()
	{
		this->!DepthTarget2D();
	}

	DepthTarget2D::!DepthTarget2D()
	{
		delete _internal;
	}

} /// GAL
} /// Lucid
