#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include <lucid/orbit/Elements.h>
#include <lucid/orbit/Properties.h>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

#define theEphemeris (LUCID_ORBIT::Ephemeris::instance())

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	EphemerisEntry::EphemerisEntry(LUCID_ORBIT::Ephemeris::Entry const &entry)
	{
		_internal = new LUCID_ORBIT::Ephemeris::Entry(entry);
	}

	EphemerisEntry::~EphemerisEntry()
	{
		this->!EphemerisEntry();
	}

	EphemerisEntry::!EphemerisEntry()
	{
		delete _internal;
	}

	EphemerisEntryType EphemerisEntry::Type::get()
	{
			return EphemerisEntryType(_internal->type);
	}

	void EphemerisEntry::Type::set(EphemerisEntryType value)
	{
		_internal->type = LUCID_ORBIT::Ephemeris::Entry::TYPE(value);
	}

	System::String ^EphemerisEntry::Name::get()
	{
		return MI::marshal_as<System::String ^>(_internal->name);
	}

	void EphemerisEntry::Name::set(System::String ^value)
	{
		_internal->name = MI::marshal_as<std::string>(value);
	}

	System::String ^EphemerisEntry::Description::get()
	{
		return MI::marshal_as<System::String ^>(_internal->description);
	}

	void EphemerisEntry::Description::set(System::String ^value)
	{
		_internal->description = MI::marshal_as<std::string>(value);
	}

	/// 
	/// 
	/// 

	void Ephemeris::Initialize(System::String ^path)
	{
		theEphemeris.initialize(MI::marshal_as<std::string>(path));
	}

	void Ephemeris::Shutdown()
	{
		theEphemeris.shutdown();
	}

	EphemerisEntry ^Ephemeris::LookupEntry(System::String ^name)
	{
		LUCID_ORBIT::Ephemeris::Entry entry;
		if (theEphemeris.lookup(entry, MI::marshal_as<std::string>(name)))
			return gcnew EphemerisEntry(entry);

		return nullptr;
	}

	EphemerisEntry ^Ephemeris::LookupEntry(size_t id)
	{
		LUCID_ORBIT::Ephemeris::Entry entry;
		if (theEphemeris.lookup(entry, id))
			return gcnew EphemerisEntry(entry);

		return nullptr;
	}

	PhysicalProperties ^Ephemeris::LookupProperties(System::String ^name)
	{
		LUCID_ORBIT::PhysicalProperties properties;
		if (theEphemeris.lookup(properties, MI::marshal_as<std::string>(name)))
			return gcnew PhysicalProperties(properties);

		return nullptr;
	}

	PhysicalProperties ^Ephemeris::LookupProperties(size_t id)
	{
		LUCID_ORBIT::PhysicalProperties properties;
		if (theEphemeris.lookup(properties, id))
			return gcnew PhysicalProperties(properties);

		return nullptr;
	}

	Elements ^Ephemeris::LookupElements(System::String ^name, double jdn)
	{
		LUCID_ORBIT::Elements elements;
		if (theEphemeris.lookup(elements, MI::marshal_as<std::string>(name), LUCID_ORBIT::scalar_t(jdn)))
			return gcnew Elements(elements);

		return nullptr;
	}

	Elements ^Ephemeris::LookupElements(size_t id, double jdn)
	{
		LUCID_ORBIT::Elements elements;
		if (theEphemeris.lookup(elements, id, LUCID_ORBIT::scalar_t(jdn)))
			return gcnew Elements(elements);

		return nullptr;
	}

}	/// Orbit
}	/// Lucid
