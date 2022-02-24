#include "DepthTarget2D.h"
#include <lucid/gal/DepthTarget2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}

namespace lucid {

	DepthTarget2D::DepthTarget2D(DepthTargetFormat format, int width,  int height)
	{
		try
		{
			_internal = gal::DepthTarget2D::create(static_cast<gal::DepthTarget2D::FORMAT>(format), width, height);
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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

}	///  lucid
