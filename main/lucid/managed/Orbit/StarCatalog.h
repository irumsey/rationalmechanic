#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/StarCatalog.h>
#include <lucid/managed/Core/Utility.h>

namespace Lucid {
namespace Orbit {

	///
	///
	/// 
	public ref class StarCatalogEntry
	{
	public:
		StarCatalogEntry(LUCID_ORBIT::StarCatalog::Entry const &entry);

		~StarCatalogEntry();

		!StarCatalogEntry();

		LUCID_MANAGED_PROPERTY_EX( XNO,             xno,      size_t)
		LUCID_MANAGED_PROPERTY_EX(  RA, right_ascension,      double)
		LUCID_MANAGED_PROPERTY_EX( DEC,     declination,      double)
		LUCID_MANAGED_PROPERTY_EX( Mag,       magnitude,       float)

		property System::String ^Type { System::String ^get(); }

		property LUCID_ORBIT::StarCatalog::Entry const &ref { LUCID_ORBIT::StarCatalog::Entry const &get() { return *ptr; } }

		property LUCID_ORBIT::StarCatalog::Entry const *ptr { LUCID_ORBIT::StarCatalog::Entry const *get() { return _internal; } }

	private:
		LUCID_ORBIT::StarCatalog::Entry *_internal = nullptr;

	};

	///
	///
	/// 
	public ref class StarCatalog
	{
	public:
		~StarCatalog() {}

		!StarCatalog() {}

		static void Initialize(System::String ^path);

		static void Shutdown();

		static size_t Count();

		static StarCatalogEntry ^At(size_t index);

	protected:
		StarCatalog() {}

	};

}	///	Orbit
}	/// Lucid

