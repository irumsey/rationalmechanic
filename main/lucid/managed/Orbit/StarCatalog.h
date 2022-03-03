#pragma once

#include <lucid/orbit/StarCatalog.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(alias, name, type) property type alias { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit {

	///
	///
	/// 
	public ref class StarCatalogEntry
	{
	public:
		StarCatalogEntry(::lucid::orbit::StarCatalog::Entry const &entry);

		~StarCatalogEntry();

		!StarCatalogEntry();

		PROPERTY( XNO,             xno,      size_t)
		PROPERTY(  RA, right_ascension,      double)
		PROPERTY( DEC,     declination,      double)
		PROPERTY( Mag,       magnitude,       float)

		property System::String ^Type { System::String ^get(); }

		property ::lucid::orbit::StarCatalog::Entry const &ref { ::lucid::orbit::StarCatalog::Entry const &get() { return *ptr; } }

		property ::lucid::orbit::StarCatalog::Entry const *ptr { ::lucid::orbit::StarCatalog::Entry const *get() { return _internal; } }

	private:
		::lucid::orbit::StarCatalog::Entry *_internal = nullptr;

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

#undef PROPERTY
#pragma pop_macro("PROPERTY")
