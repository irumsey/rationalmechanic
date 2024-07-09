#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <lucid/core/Defines.h>

LUCID_CORE_BEGIN

///	Cache
///
///	K->T map for caching instances for which sharing is desired.
///	Uses the std::shared_ptr<> callback-on-release facility.
template<typename K, typename T> class Cache
{
public:
	Cache()
	{
	}

	virtual ~Cache()
	{
		assert(_cache.empty() && "cache not empty");
	}

	///	Find instance using key.
	///	If the key value exists the associated shared instance is returned.
	///	Otherwise, the supplied constructor is used to make a new instance.
	template<typename Ctor> inline std::shared_ptr<T> get(K const &key, Ctor const &ctor)
	{
		auto iter = _cache.find(key);
		if (iter != _cache.end())
			return iter->second.lock();

		std::shared_ptr<T> entry(ctor(), Dtor(_cache, key));
		_cache[key] = entry;

		return entry;
	}

private:
	typedef typename K key_t;
	typedef typename T value_t;
	typedef typename std::unordered_map<K, std::weak_ptr<T> > cache_t;

	///	Dtor
	///
	///	Destructor used to destroy an instance when the reference count
	///	for an instance goes to zero.  Removes the instance from the cache.
	struct Dtor
	{
		cache_t &cache;
		key_t key;

		Dtor(cache_t &cache, key_t const &key)
			: cache(cache)
			, key(key)
		{
		}

		inline void operator()(value_t* ptr)
		{
			assert(cache.end() != cache.find(key) && "item not in cache");
			cache.erase(key);
			delete ptr;
		}
	};

	cache_t _cache;
};

LUCID_CORE_END