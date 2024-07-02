#pragma once

#include <cassert>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncreatable.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

///	bits
///
///	utility class for counting set bits and hamming distance
class bits
{
public:
	template<typename T> static size_t setcnt(T rhs)
	{
		static_assert(false, "type not supported");
	}

	template<> static size_t setcnt<uint8_t>(uint8_t rhs)
	{
		return _setcnt[rhs];
	}

	template<> static size_t setcnt<uint16_t>(uint16_t rhs)
	{
		return setcnt<uint8_t>(0x00ff & rhs) + setcnt<uint8_t>((0xff00 & rhs) >> 8);
	}

	template<> static size_t setcnt<uint32_t>(uint32_t rhs)
	{
		return setcnt<uint16_t>(0x0000ffff & rhs) + setcnt<uint16_t>((0xffff0000 & rhs) >> 16);
	}

	template<typename T> static size_t hamming_distance(T lhs, T rhs)
	{
		return setcnt<T>(lhs ^ rhs);
	}

private:
	static size_t const _setcnt[0x100];

	LUCID_PREVENT_CREATION(bits);
};

LUCID_CORE_END
