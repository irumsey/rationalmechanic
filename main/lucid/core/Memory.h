#pragma once

#include <lucid/core/Defines.h>

LUCID_CORE_BEGIN

///	reset_raw_ptr 
///
/// delete and set to null
template<typename T> inline void reset_raw_ptr(T *&ptr)
{
	delete ptr;
	ptr = nullptr;
}

LUCID_CORE_END
