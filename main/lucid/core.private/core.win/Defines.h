#pragma once

#ifndef CORE_PRIVATE
#	error core layer violation
#endif

#include <lucid/core/Defines.h>

#define LUCID_CORE_WIN ::lucid::core::win

#define LUCID_CORE_WIN_BEGIN LUCID_CORE_BEGIN namespace win {
#define LUCID_CORE_WIN_END LUCID_CORE_END }
