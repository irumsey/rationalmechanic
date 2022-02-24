#pragma once

namespace lucid {
namespace gal  {

	class Parameter;

}	///	gal
}	///	lucid

namespace lucid {

	///	Parameter
	///
	///
	public ref class Parameter
	{
	public:
		Parameter(::lucid::gal::Parameter const *parameter);

		~Parameter();

		!Parameter();

		property System::String ^name { System::String ^get(); }

		property ::lucid::gal::Parameter const &ref { ::lucid::gal::Parameter const &get() { return *ptr; } }

		property ::lucid::gal::Parameter const *ptr { ::lucid::gal::Parameter const *get() { return _internal; } }

	private:
		::lucid::gal::Parameter const *_internal = nullptr;

	};

}	///  lucid
