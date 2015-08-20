#include "Parameter.h"

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	Parameter::Parameter(Uniform::TYPE type, std::string const &name)
		: type(type)
		, _name(name)
	{
	}

}	///	d3d11
}	///	gal
}	///	lucid