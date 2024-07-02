#pragma once

#include <unordered_map>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Error.h>
#include <lucid/core/Ctor.h>

LUCID_CORE_BEGIN

///	Factory
///
///	general factory template.  map of keys to constructors.
template<class K, class T> class Factory
{
public:
	Factory() = default;

	virtual ~Factory()
	{
		for (auto iter : _ctors) { delete iter.second; }
	}

	void add(K const &key, Ctor<T> const *ctor)
	{
		LUCID_VALIDATE(_ctors.end() == _ctors.find(key), "duplicate key specified");
		_ctors[key] = ctor;
	}

	T *create(K const &key) const
	{
		auto entry = _ctors.find(key);
		LUCID_VALIDATE(entry != _ctors.end(), "unknown key specified");

		Ctor<T> const *ctor = entry->second;
		return ctor->create();
	}

private:
	std::unordered_map<K, Ctor<T> const *> _ctors;

	LUCID_PREVENT_COPY(Factory);
	LUCID_PREVENT_ASSIGNMENT(Factory);
};

LUCID_CORE_END