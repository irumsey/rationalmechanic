#include "RenderTarget2D.h"
#include <lucid/gal/RenderTarget2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}

namespace Lucid {
namespace GAL {

	RenderTarget2D::RenderTarget2D(RenderTargetFormat format, int width, int height)
	{
		try
		{
			_internal = new shared_ptr_t(gal::RenderTarget2D::create(static_cast<gal::RenderTarget2D::FORMAT>(format), width, height));
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
			throw;
		}
	}

	RenderTarget2D::~RenderTarget2D()
	{
		this->!RenderTarget2D();
	}

	RenderTarget2D::!RenderTarget2D()
	{
		delete _internal;
	}

	int RenderTarget2D::width::get()
	{
		return ref.width();
	}

	int RenderTarget2D::height::get()
	{
		return ref.height();
	}

} /// GAL
} /// Lucid
