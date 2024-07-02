#include "Parameter.h"
#include <lucid/gal/Parameter.h>
#include <msclr/marshal_cppstd.h>

LUCID_ANONYMOUS_BEGIN

namespace MI = msclr::interop;

LUCID_ANONYMOUS_END

namespace Lucid {
namespace GAL {

	Parameter::Parameter(LUCID_GAL::Parameter const *parameter)
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
		return MI::marshal_as<System::String ^>(_internal->name());
	}

} /// GIGL
} /// Lucid
