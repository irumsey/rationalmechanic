#pragma once

#include <vector>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
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

	template<typename T> T read();

	template<typename T, typename I> T read_counted(I expected);

	void member_begin(char const *comment = nullptr);

	void member_end(char const *comment = nullptr);

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

template<typename T> inline T Unserializer::read()
{
	return Reader::read<T>();
}

template<typename T, typename I> T Unserializer::read_counted(I expected)
{
	I actual = read<I>();
	// error if expected != actual
	return read<T>();
}

LUCID_CORE_END
