#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Parameter.h>
#include <lucid/gal.private/gal.d3d11/Defines.h>
#include <lucid/gal.private/gal.d3d11/Shader.h>

LUCID_GAL_D3D11_BEGIN

///
///
///
class Parameter : public LUCID_GAL::Parameter
{
public:
	Uniform::TYPE type = Uniform::TYPE_UNDEFINED;

	int32_t vsPosition = -1;
	int32_t gsPosition = -1;
	int32_t psPosition = -1;

	int32_t size = 0;

	Parameter() = default;

	Parameter(Uniform::TYPE type, std::string const &name);

	virtual ~Parameter() = default;

	virtual std::string const &name() const override;

private:
	std::string _name = "<undefined>";

	LUCID_PREVENT_COPY(Parameter);
	LUCID_PREVENT_ASSIGNMENT(Parameter);
};

inline std::string const &Parameter::name() const
{
	return _name;
}

LUCID_GAL_D3D11_END