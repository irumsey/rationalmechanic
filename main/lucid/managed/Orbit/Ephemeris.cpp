#include "Ephemeris.h"
#include "Elements.h"
#include "Properties.h"
#include <lucid/orbit/Elements.h>
#include <lucid/orbit/Properties.h>
#include <msclr/marshal_cppstd.h>

namespace /* anonymous */ {

	namespace    MI = msclr::interop;
	namespace orbit = ::lucid::orbit;

}

#define theEphemeris (orbit::Ephemeris::instance())

namespace Lucid {
namespace Orbit {

	/// 
	/// 
	/// 

	EphemerisEntry::EphemerisEntry(orbit::Ephemeris::Entry const &entry)
	{
		_internal = new orbit::Ephemeris::Entry(entry);
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
		_internal->type = ::lucid::orbit::Ephemeris::Entry::TYPE(value);
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
		orbit::Ephemeris::Entry entry;
		if (theEphemeris.lookup(entry, MI::marshal_as<std::string>(name)))
			return gcnew EphemerisEntry(entry);

		return nullptr;
	}

	EphemerisEntry ^Ephemeris::LookupEntry(size_t id)
	{
		orbit::Ephemeris::Entry entry;
		if (theEphemeris.lookup(entry, id))
			return gcnew EphemerisEntry(entry);

		return nullptr;
	}

	PhysicalProperties ^Ephemeris::LookupProperties(System::String ^name)
	{
		orbit::PhysicalProperties properties;
		if (theEphemeris.lookup(properties, MI::marshal_as<std::string>(name)))
			return gcnew PhysicalProperties(properties);

		return nullptr;
	}

	PhysicalProperties ^Ephemeris::LookupProperties(size_t id)
	{
		orbit::PhysicalProperties properties;
		if (theEphemeris.lookup(properties, id))
			return gcnew PhysicalProperties(properties);

		return nullptr;
	}

	Elements ^Ephemeris::LookupElements(System::String ^name, double jdn)
	{
		orbit::Elements elements;
		if (theEphemeris.lookup(elements, MI::marshal_as<std::string>(name), orbit::scalar_t(jdn)))
			return gcnew Elements(elements);

		return nullptr;
	}

	Elements ^Ephemeris::LookupElements(size_t id, double jdn)
	{
		orbit::Elements elements;
		if (theEphemeris.lookup(elements, id, orbit::scalar_t(jdn)))
			return gcnew Elements(elements);

		return nullptr;
	}

}	/// Orbit
}	/// Lucid
