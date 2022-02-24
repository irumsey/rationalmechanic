#include "TargetReader2D.h"
#include "RenderTarget2D.h"
#include <lucid/gal/TargetReader2D.h>

namespace {

	namespace gal = ::lucid::gal;

}	///	anonymous

namespace lucid {

	TargetReader2D::TargetReader2D(RenderTarget2D ^target)
	{
		_internal = gal::TargetReader2D::create(target->ptr, target->width, target->height);
	}

	TargetReader2D::~TargetReader2D()
	{
		this->!TargetReader2D();
	}

	TargetReader2D::!TargetReader2D()
	{
		delete _internal;
	}

	unsigned int TargetReader2D::sample(int x, int y)
	{
		unsigned int const *value = (unsigned int const *)(_internal->sample(x, y, 1, 1));
		return *value;
	}

}	///  lucid
