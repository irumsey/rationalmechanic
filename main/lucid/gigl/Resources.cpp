#include "Resources.h"

///	there are issues if lucid is used with C#
///	the gc interferes shutdown order
/// #define GIGL_DELETE_RESOURCES

LUCID_GIGL_BEGIN

Resources::Resources()
{
}

Resources::~Resources()
{
#if defined(GIGL_DELETE_RESOURCES)
	for (auto iter : _caches) { delete iter.second; }
#endif
}

Resources &Resources::instance()
{
	static Resources theInstance;
	return theInstance;
}

LUCID_GIGL_END