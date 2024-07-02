#pragma once

#include <lucid/gal/Defines.h>

LUCID_GAL_BEGIN

class Parameter;

LUCID_GAL_END

namespace Lucid {
namespace GAL {

	///	Parameter
	///
	///
	public ref class Parameter
	{
	public:
		Parameter(LUCID_GAL::Parameter const *parameter);

		~Parameter();

		!Parameter();

		property System::String ^name { System::String ^get(); }

		property LUCID_GAL::Parameter const &ref { LUCID_GAL::Parameter const &get() { return *ptr; } }

		property LUCID_GAL::Parameter const *ptr { LUCID_GAL::Parameter const *get() { return _internal; } }

	private:
		LUCID_GAL::Parameter const *_internal = nullptr;

	};

} /// GIGL
} /// Lucid
