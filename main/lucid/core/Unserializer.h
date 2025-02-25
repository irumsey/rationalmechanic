#pragma once

#include <vector>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <lucid/core/Reader.h>

LUCID_CORE_BEGIN

class FileReader;

/// 
/// 
/// 
class Unserializer : public Reader
{
public:
	Unserializer() = default;

	Unserializer(std::string const &path);

	virtual ~Unserializer();

	bool open(std::string const &path);

	bool is_open() const;

	bool not_open() const;

	void close();

	void read(void *data, size_t size) override;

	template<typename T> void expect(T const &expected);

	template<typename T> T read();

	template<typename T> T read(int32_t expected);

	void nested_begin(char const *comment = nullptr);

	void nested_end(char const *comment = nullptr);

private:
	std::vector<FileReader*> _stack;
	FileReader *_current = nullptr;

	LUCID_PREVENT_ASSIGNMENT(Unserializer);
	LUCID_PREVENT_COPY(Unserializer);
};

inline bool Unserializer::is_open() const
{
	return (nullptr != _current);
}

inline bool Unserializer::not_open() const
{
	return !is_open();
}

template<typename T> inline void Unserializer::expect(T const &expected)
{
	T actual = read<T>();
	LUCID_VALIDATE(actual == expected, "expected value does not match actual");
}

template<typename T> inline T Unserializer::read()
{
	return Reader::read<T>();
}

template<typename T> inline T Unserializer::read(int32_t expected)
{
	expect(expected);
	return read<T>();
}

LUCID_CORE_END
