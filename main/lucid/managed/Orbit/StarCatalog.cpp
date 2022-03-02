#include "StarCatalog.h"
#include <lucid/orbit/Ephemeris.h>
#include <msclr/marshal_cppstd.h>

namespace /* anonymous */ {

	namespace    MI = msclr::interop;
	namespace orbit = ::lucid::orbit;

}

#define theCatalog (orbit::StarCatalog::instance())

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	StarCatalogEntry::StarCatalogEntry(orbit::StarCatalog::Entry const &entry)
	{
		_internal = new orbit::StarCatalog::Entry(entry);
	}

	StarCatalogEntry::~StarCatalogEntry()
	{
		this->!StarCatalogEntry();
	}

	StarCatalogEntry::!StarCatalogEntry()
	{
		delete _internal;
	}

	/// 
	/// 
	/// 

	void StarCatalog::Initialize(System::String ^path)
	{
		theCatalog.initialize(MI::marshal_as<std::string>(path));
	}

	void StarCatalog::Shutdown()
	{
		theCatalog.shutdown();
	}

	size_t StarCatalog::Count()
	{
		return theCatalog.count();
	}

	StarCatalogEntry ^StarCatalog::At(size_t index)
	{
		return gcnew StarCatalogEntry(theCatalog[index]);
	}

}	/// Orbit
}	/// Lucid
