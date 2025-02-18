#pragma once

#include <string>
#include <unordered_map>
#include <lucid/core/Error.h>
#include <rnd/_orbit/Defines.h>

ORBIT_BEGIN

class Frame;

/// Dictionary
/// 
/// wrapper for an unordered map to map names to frames
class Dictionary
{
public:
	Dictionary()
	{
		_internal["__null__"] = nullptr;
	}

	virtual ~Dictionary() = default;

	void define(std::string const &name, Frame *frame)
	{
		LUCID_VALIDATE(_internal.end() == _internal.find(name), "'" + name + "' already exists in dictionary");
		_internal[name] = frame;
	}

	Frame *operator[](std::string const &name) const
	{
		return lookup(name);
	}

	Frame *lookup(std::string const &name) const
	{
		auto iter = _internal.find(name);
		LUCID_VALIDATE(iter != _internal.end(), "unknown frame '" + name + "' requested");

		return iter->second;
	}

	void clear()
	{
		_internal.clear();
	}

private:
	std::unordered_map<std::string, Frame *> _internal;

};

ORBIT_END
