#pragma once

#include <string>
#include <lucid/core/Noncopyable.h>

///
///
///
namespace lucid {
namespace gal {

	///	Parameter
	///
	///	opaque gal program parameter used to "push" data
	///	to a program.
	///
	///	example:
	///		Parameter const *myFloatParameter = program->lookup("myParameter");
	///		/// ...
	///		program->set(myFloatParameter, 5.f);
	///
	///	note: Program retains ownership of the returned parameter.  it, the parameter,
	///	is valid for the life of Program.
	class Parameter
	{
	public:
		virtual ~Parameter() = 0 {}

		virtual std::string const &name() const = 0;

	protected:
		Parameter() {}

		LUCID_PREVENT_COPY(Parameter);
		LUCID_PREVENT_ASSIGNMENT(Parameter);
	};

}	///	gal
}	///	lucid
