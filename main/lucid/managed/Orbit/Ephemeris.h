#pragma once

#include <lucid/orbit/Ephemeris.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(alias, name, type) property type alias { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit {

	ref class PhysicalProperties;
	ref class Elements;

	///
	/// 
	/// 
	public enum class EphemerisEntryType
	{
		TYPE_UNDEFINED     = ::lucid::orbit::Ephemeris::Entry::TYPE_UNDEFINED,
		TYPE_DYNAMIC_POINT = ::lucid::orbit::Ephemeris::Entry::TYPE_DYNAMIC_POINT,
		TYPE_ORBITAL_BODY  = ::lucid::orbit::Ephemeris::Entry::TYPE_ORBITAL_BODY,
		TYPE_DYNAMIC_BODY  = ::lucid::orbit::Ephemeris::Entry::TYPE_DYNAMIC_BODY,
	};

	///
	///
	/// 
	public ref class EphemerisEntry
	{
	public:
		EphemerisEntry(::lucid::orbit::Ephemeris::Entry const &entry);

		~EphemerisEntry();

		!EphemerisEntry();

		PROPERTY(       Type,        type, ::lucid::orbit::Ephemeris::Entry::TYPE)
		PROPERTY(         ID,          id,                                 size_t)
		PROPERTY(        CID,         cid,                                 size_t)
		PROPERTY(       Name,        name,                            std::string)
		PROPERTY(Description, description,                            std::string)

		property ::lucid::orbit::Ephemeris::Entry const &ref { ::lucid::orbit::Ephemeris::Entry const &get() { return *ptr; } }

		property ::lucid::orbit::Ephemeris::Entry const *ptr { ::lucid::orbit::Ephemeris::Entry const *get() { return _internal; } }

	private:
		::lucid::orbit::Ephemeris::Entry *_internal = nullptr;

	};

	///
	///
	/// 
	public ref class Ephemeris
	{
	public:
		~Ephemeris() {}

		!Ephemeris() {}

		static void Initialize(System::String ^path);

		static void Shutdown();

		static EphemerisEntry ^LookupEntry(System::String ^name);

		static EphemerisEntry ^LookupEntry(size_t id);

		static PhysicalProperties ^LookupProperties(System::String ^name);

		static PhysicalProperties ^LookupProperties(size_t id);

		static Elements ^LookupElements(System::String ^name, double jdn);

		static Elements ^LookupElements(size_t id, double jdn);

	protected:
		Ephemeris() {}

	};

}	///	Orbit
}	/// Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
