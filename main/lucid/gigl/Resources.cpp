#include "Resources.h"

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
		for (auto iter : _caches) { delete iter.second; }
	}

	Resources &Resources::instance()
	{
		static Resources theInstance;
		return theInstance;
	}

}	///	gigl
}	///	lucid