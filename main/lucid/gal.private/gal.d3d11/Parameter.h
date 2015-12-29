#pragma once

#if !defined(GAL_PRIVATE)
#   error gal layer violation
#endif

#include <lucid/core/Types.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/gal/Parameter.h>
#include <lucid/gal.private/gal.d3d11/Shader.h>

///
///
///
namespace lucid {
namespace gal {
namespace d3d11 {

	///
	///
	///
	class Parameter : public ::lucid::gal::Parameter
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

}	///	d3d11
}	///	gal
}	///	lucid