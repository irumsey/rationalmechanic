#pragma once

#include <lucid/orbit/Defines.h>
#include <lucid/orbit/Selection.h>
#include <lucid/managed/Core/Utility.h>

namespace Lucid {
namespace Orbit{

	///	
	///
	///

	public enum class SelectionOrigin
	{
		ORIGIN_NATURAL		= LUCID_ORBIT::Selection::ORIGIN_NATURAL,
		ORIGIN_ARTIFICIAL	= LUCID_ORBIT::Selection::ORIGIN_ARTIFICIAL,
	};

	public enum class SelectionType
	{
		TYPE_NONE			= LUCID_ORBIT::Selection::TYPE_NONE,
		TYPE_STAR			= LUCID_ORBIT::Selection::TYPE_STAR,
		TYPE_ORBIT			= LUCID_ORBIT::Selection::TYPE_ORBIT,
		TYPE_FRAME			= LUCID_ORBIT::Selection::TYPE_FRAME,
		TYPE_CAMERA			= LUCID_ORBIT::Selection::TYPE_CAMERA,
		TYPE_ICON			= LUCID_ORBIT::Selection::TYPE_ICON,
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

		property int ID { int get() { return _internal->id(); }}

		LUCID_MANANGED_ENUMERATION(Origin, origin, SelectionOrigin, LUCID_ORBIT::Selection::ORIGIN)
		LUCID_MANANGED_ENUMERATION(  Type,   type,   SelectionType, LUCID_ORBIT::Selection::  TYPE)
		LUCID_MANAGED_PROPERTY_EX ( Token,  token,       uint32_t)

		property LUCID_ORBIT::Selection const &ref { LUCID_ORBIT::Selection const &get() { return *ptr; } }

		property LUCID_ORBIT::Selection const *ptr { LUCID_ORBIT::Selection const *get() { return _internal; } }

	private:
		LUCID_ORBIT::Selection *_internal = nullptr;

	};

}	///	Orbit
}	///	Lucid
