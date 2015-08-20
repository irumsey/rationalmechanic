#pragma once

#include <cassert>
#include <memory>
#include <hash_map>

///
///
///
namespace lucid {
namespace core {

	///	Cache
	///
	///	K->T map for caching instances for which sharing is desired.
	///	Uses the std::shared_ptr<> callback-on-release facility.
	template<class K, class T> class Cache
	{
	public:
		Cache()
		{
		}

		virtual ~Cache()
		{
			assert(cache.empty() && "cache not empty");
		}

		///	Find instance using key.
		///	If the key value exists the associated shared instance is returned.
		///	Otherwise, the supplied constructor is used to make a new instance.
		template<class C> inline std::shared_ptr<T> get(K const &key, C const &ctor)
		{
			std::hash_map<K, std::weak_ptr<T> >::iterator iter = cache.find(key);
			if (iter != cache.end())
				return iter->second.lock();

			std::shared_ptr<T> entry(ctor(), Dtor(cache, key));
			cache[key] = entry;

			return entry;
		}

	private:
		typedef K key_t;
		typedef T value_t;
		typedef std::hash_map<K, std::weak_ptr<T> > cache_t;

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

		cache_t cache;
	};

}	///	core
}	///	lucid