#include "Resources.h"

///	there are issues if lucid is used with C#
///	the gc screws up shutdown order
///	#define GIGL_DELETE_RESOURCES

///
///
///
namespace lucid {
namespace gigl {

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

}	///	gigl
}	///	lucid