#include "Parameter.h"
#include <lucid/gal/Parameter.h>
#include <msclr/marshal_cppstd.h>

namespace /* anonymous */
{

	namespace MI = msclr::interop;
	namespace gal = ::lucid::gal;

}

namespace Lucid {
namespace GAL {

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
		return MI::marshal_as<System::String ^>(_internal->name());
	}

} /// GIGL
} /// Lucid
