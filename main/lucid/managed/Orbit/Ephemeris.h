#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Ephemeris.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(alias, name, type) property type alias { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit {

	ref class PhysicalProperties;
	ref class OrbitalElements;

	///
	/// 
	/// 
	public enum class EphemerisEntryType
	{
		TYPE_UNDEFINED     = LUCID_ORBIT::Ephemeris::Entry::TYPE_UNDEFINED,
		TYPE_DYNAMIC_POINT = LUCID_ORBIT::Ephemeris::Entry::TYPE_DYNAMIC_POINT,
		TYPE_ORBITAL_BODY  = LUCID_ORBIT::Ephemeris::Entry::TYPE_ORBITAL_BODY,
		TYPE_DYNAMIC_BODY  = LUCID_ORBIT::Ephemeris::Entry::TYPE_DYNAMIC_BODY,
	};

	///
	///
	/// 
	public ref class EphemerisEntry
	{
	public:
		EphemerisEntry(LUCID_ORBIT::Ephemeris::Entry const &entry);

		~EphemerisEntry();

		!EphemerisEntry();

		PROPERTY( ID,  id, size_t)
		PROPERTY(CID, cid, size_t)

		property EphemerisEntryType Type {
			EphemerisEntryType get();
			void set(EphemerisEntryType value);
		}

		property System::String ^Name {
			System::String ^get();
			void set(System::String ^value);
		}

		property System::String ^Description {
			System::String ^get();
			void set(System::String ^value);
		}

		property LUCID_ORBIT::Ephemeris::Entry const &ref { LUCID_ORBIT::Ephemeris::Entry const &get() { return *ptr; } }

		property LUCID_ORBIT::Ephemeris::Entry const *ptr { LUCID_ORBIT::Ephemeris::Entry const *get() { return _internal; } }

	private:
		LUCID_ORBIT::Ephemeris::Entry *_internal = nullptr;

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

		static OrbitalElements ^LookupElements(System::String ^name, double jdn);

		static OrbitalElements ^LookupElements(size_t id, double jdn);

	protected:
		Ephemeris() {}

	};

}	///	Orbit
}	/// Lucid

#undef PROPERTY
#pragma pop_macro("PROPERTY")
