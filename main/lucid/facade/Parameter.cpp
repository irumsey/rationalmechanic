#include "Parameter.h"
#include <lucid/gal/Parameter.h>
#include <msclr/marshal_cppstd.h>

namespace MI = msclr::interop;

namespace /* anonymous */
{

	namespace gal = ::lucid::gal;

}

namespace lucid {

	Parameter::Parameter(gal::Parameter const *parameter)
		: _internal(parameter)
	{
	}

	Parameter::~Parameter()
	{
		this->!Parameter();
	}

	Parameter::!Parameter()
	{
		///	NOP: program owns the internal parameter
	}

	System::String ^Parameter::name::get()
	{
		return MI::marshal_as<System::String^>(_internal->name());
	}

}	///  lucid
