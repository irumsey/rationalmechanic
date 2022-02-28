#include "Unordered2D.h"
#include <lucid/gal/Unordered2D.h>
#include <lucid/core/Logger.h>
#include <lucid/core/Error.h>

namespace /* anonymous */
{

	namespace core = ::lucid::core;
	namespace  gal = ::lucid::gal;

}

namespace Lucid {
namespace GAL {

	Unordered2D::Unordered2D(int width, int height)
	{
		try
		{
			_internal = gal::Unordered2D::create(width, height);
		}
		catch (core::Error const &error)
		{
			core::log("ERROR", error.what());
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
