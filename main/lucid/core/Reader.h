#pragma once

#include <utility>
#include <string>
#include <lucid/core/Defines.h>
#include <lucid/core/Types.h>
#include <lucid/core/Identity.h>

LUCID_CORE_BEGIN

///	Reader
///
///	generic serialization facility.
class Reader
{
public:
	virtual ~Reader() = default;

	virtual void read(void *data, size_t size) = 0;

	template<class T> T read();

protected:
	Reader() = default;

};

template<class T> inline T Reader::read()
{
	T value = T();
	read(&value, sizeof(T));
	return value;
}

template<> inline std::string Reader::read<std::string>()
{
	int size = read<int>();
	std::string value(size, 0);

	if (0 != size) read(&value[0], size);

	return std::move(value);
}

template<> inline Identity Reader::read<Identity>()
{
	Identity value;
	read(value.data(), Identity::SIZE);
	return value;
}

LUCID_CORE_END
