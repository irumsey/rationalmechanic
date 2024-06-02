#pragma once

#include <lucid/orbit/Selection.h>

#pragma push_macro("PROPERTY")
#define PROPERTY(alias, name, type) property type alias { type get() { return _internal->name; } void set(type value) { _internal->name = value; } }

namespace Lucid {
namespace Orbit{

	///	
	///
	///
	public enum class SelectionType
	{
		TYPE_NONE    = 0,
		TYPE_STAR    = ::lucid::orbit::Selection::TYPE_STAR,
		TYPE_FRAME   = ::lucid::orbit::Selection::TYPE_FRAME,
		TYPE_ORBIT   = ::lucid::orbit::Selection::TYPE_ORBIT,
		TYPE_CAMERA  = ::lucid::orbit::Selection::TYPE_CAMERA,
		TYPE_CALLOUT = ::lucid::orbit::Selection::TYPE_CALLOUT,
		TYPE_OTHER   = ::lucid::orbit::Selection::TYPE_OTHER,
	};

	///
	///
	///
	public ref class Selection
	{
	public:
		Selection(::lucid::orbit::Selection const &selection);

		~Selection();

		!Selection();

		property SelectionType Type {
			SelectionType get();
			void set(SelectionType value);
		}

		PROPERTY(   Token,   token,      uint32_t)

		property ::lucid::orbit::Selection const &ref { ::lucid::orbit::Selection const &get() { return *ptr; } }

		property ::lucid::orbit::Selection const *ptr { ::lucid::orbit::Selection const *get() { return _internal; } }

	private:
		::lucid::orbit::Selection *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid


#undef PROPERTY
#pragma pop_macro("PROPERTY")
