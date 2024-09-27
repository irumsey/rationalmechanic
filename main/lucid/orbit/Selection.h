#pragma once

#include <cmath>
#include <lucid/core/Types.h>
#include <lucid/orbit/Defines.h>

LUCID_ORBIT_BEGIN

///	Selection
///
/// 
struct Selection
{
	/// ORIGIN
	/// 
	/// 
	enum ORIGIN
	{
		ORIGIN_NATURAL		= 0x00,
		ORIGIN_ARTIFICIAL	= 0x01,
	};

	/// TYPE
	/// 
	/// 
	enum TYPE
	{
		TYPE_NONE			= 0x00,
		TYPE_STAR			= 0x01,
		TYPE_ORBIT			= 0x02,
		TYPE_FRAME			= 0x03,
		TYPE_CAMERA			= 0x04,
		TYPE_ICON			= 0x05,
		TYPE_UNDEFINED_0	= 0x06,
		TYPE_UNDEFINED_1	= 0x07,
	};

	enum { TYPE_SHIFT		= 28 };
	enum { ORIGIN_SHIFT		= 31 };

	enum { ORIGIN_MASK		= 0x80000000 };
	enum { TYPE_MASK		= 0x70000000 };
	enum { ID_MASK			= 0x0fffffff };

	ORIGIN origin	= ORIGIN_NATURAL;
	TYPE type		= TYPE_NONE;
	uint32_t token	= 0;

	Selection() = default;

	Selection(uint32_t token)
		: token(token)
	{
		origin = (ORIGIN_MASK & token) ? ORIGIN_ARTIFICIAL : ORIGIN_NATURAL;
		type = TYPE((TYPE_MASK & token) >> TYPE_SHIFT);
	}

	Selection(TYPE type, int32_t id)
	{
		origin = (id < 0) ? ORIGIN_ARTIFICIAL : ORIGIN_NATURAL;
		token = hash(type, id);
	}

	~Selection() = default;

	int32_t id() const
	{
		int32_t value = ID_MASK & token;
		return ((ORIGIN_ARTIFICIAL == origin) ? -value : value);
	}

	static uint32_t hash(TYPE type, int32_t id)
	{
		ORIGIN origin = (id < 0) ? ORIGIN_ARTIFICIAL : ORIGIN_NATURAL;
		return (origin << ORIGIN_SHIFT) | (type << TYPE_SHIFT) | std::abs(id);
	}
};

LUCID_ORBIT_END
