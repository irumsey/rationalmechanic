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

	unsigned int TargetReader2D::read(int x, int y)
	{
		unsigned int const *value = (unsigned int const *)(_internal->read());
		return value[y * _internal->width() + x];
	}

}	///  lucid
