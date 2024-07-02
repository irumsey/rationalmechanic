#pragma once

#include <string>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

///
///
///

class Identity;

///	Writer
///
///	generic serialization facility.
class Writer
{
public:
	virtual ~Writer() = 0 {}

	virtual void write(void const *data, int size) = 0;

	virtual void write(bool data) = 0;

	virtual void write(int8_t data) = 0;

	virtual void write(uint8_t data) = 0;

	virtual void write(int16_t data) = 0;

	virtual void write(uint16_t data) = 0;

	virtual void write(int32_t data) = 0;

	virtual void write(uint32_t data) = 0;

	virtual void write(int64_t data) = 0;

	virtual void write(uint64_t data) = 0;

	virtual void write(float32_t data) = 0;

	virtual void write(float64_t data) = 0;

	virtual void write(char const *data) = 0;

	virtual void write(std::string const &data) = 0;

	virtual void write(Identity const &data) = 0;

protected:
	Writer() {}

	LUCID_PREVENT_COPY(Writer);
	LUCID_PREVENT_ASSIGNMENT(Writer);
};

LUCID_CORE_END