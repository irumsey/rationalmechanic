#pragma once

#include <string>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Identity.h>

LUCID_CORE_BEGIN

///	Writer
///
///	generic serialization facility.
class Writer
{
public:
	virtual ~Writer() = 0 {}

	virtual void write(void const *data, size_t size) = 0;

	template<typename T> void write(T const &value);

protected:
	Writer() {}

	LUCID_PREVENT_COPY(Writer);
	LUCID_PREVENT_ASSIGNMENT(Writer);
};

template<typename T> inline void Writer::write(T const &value)
{
	write(&value, sizeof(T));
}

template<> inline void Writer::write<std::string>(std::string const &value)
{
	int32_t size = int32_t(value.size());
	write(size);
	if (0 != size) write(&value[0], size);
}

template<> inline void Writer::write<cstr_t>(cstr_t const &value)
{
	int32_t size = int32_t(::strlen(value));
	write(size);
	if (0 != size) write(value, size);
}

template<> inline void Writer::write<Identity>(Identity const &value)
{
	write(value.data(), Identity::SIZE);
}

LUCID_CORE_END