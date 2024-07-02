#include "StarCatalog.h"
#include <lucid/orbit/Ephemeris.h>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

#define theCatalog (LUCID_ORBIT::StarCatalog::instance())

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	StarCatalogEntry::StarCatalogEntry(LUCID_ORBIT::StarCatalog::Entry const &entry)
	{
		_internal = new LUCID_ORBIT::StarCatalog::Entry(entry);
	}

	StarCatalogEntry::~StarCatalogEntry()
	{
		this->!StarCatalogEntry();
	}

	StarCatalogEntry::!StarCatalogEntry()
	{
		delete _internal;
	}

	System::String ^StarCatalogEntry::Type::get()
	{
		return MI::marshal_as<System::String ^>(_internal->type);
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
