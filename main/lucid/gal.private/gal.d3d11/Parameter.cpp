#include "Parameter.h"

LUCID_GAL_D3D11_BEGIN

Parameter::Parameter(Uniform::TYPE type, std::string const &name)
	: type(type)
	, _name(name)
{
}

LUCID_GAL_D3D11_END