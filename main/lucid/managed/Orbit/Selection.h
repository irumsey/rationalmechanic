#pragma once

#include <lucid/orbit/Defines.h>
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
		TYPE_STAR    = LUCID_ORBIT::Selection::TYPE_STAR,
		TYPE_FRAME   = LUCID_ORBIT::Selection::TYPE_FRAME,
		TYPE_ORBIT   = LUCID_ORBIT::Selection::TYPE_ORBIT,
		TYPE_CAMERA  = LUCID_ORBIT::Selection::TYPE_CAMERA,
		TYPE_CALLOUT = LUCID_ORBIT::Selection::TYPE_CALLOUT,
		TYPE_OTHER   = LUCID_ORBIT::Selection::TYPE_OTHER,
	};

	///
	///
	///
	public ref class Selection
	{
	public:
		Selection(LUCID_ORBIT::Selection const &selection);

		~Selection();

		!Selection();

		property SelectionType Type {
			SelectionType get();
			void set(SelectionType value);
		}

		PROPERTY(   Token,   token,      uint32_t)

		property LUCID_ORBIT::Selection const &ref { LUCID_ORBIT::Selection const &get() { return *ptr; } }

		property LUCID_ORBIT::Selection const *ptr { LUCID_ORBIT::Selection const *get() { return _internal; } }

	private:
		LUCID_ORBIT::Selection *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid


#undef PROPERTY
#pragma pop_macro("PROPERTY")
