#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Ctor.h>
#include <lucid/core/Cache.h>
#include <lucid/gigl/Defines.h>

LUCID_GIGL_BEGIN

///	Resources
///
///	Note: resources are not required to share some common base.
///	the only requirement for a resource is that it define a static create(std::string const &) method.
///	in other words, the resource is created from a key.  the key which has meaning
///	to the resource being created.  same key, same resource.  the method of creation is deferred to the type.
class Resources final
{
public:
	virtual ~Resources();

	template<class T> static std::shared_ptr<T> get(std::string const &key);

	static Resources &instance();

protected:
	Resources();

private:
	///
	///
	///
	template<class T> struct Type
	{
		///	unique ID "generated" by returning the address of this function (which is
		///	unique for each Type<T> instance, unique for each T).
		///	note: internal use only, do not persist this value across application instances.
		static void* ID()
		{
			return &ID;
		}
	};

	///
	///
	///
	class CacheBase
	{
	public:
		virtual ~CacheBase() = 0 {}

	protected:
		CacheBase() {}
	};

	///
	///
	///
	template<class T> class ResourceCache : public CacheBase
	{
	public:
		ResourceCache()
		{
		}

		virtual ~ResourceCache()
		{
		}

		std::shared_ptr<T> get(std::string const &key)
		{
			return internal.get(key, LUCID_CORE::CtorResource<T>(key));
		}

	private:
		LUCID_CORE::Cache<std::string, T> internal;
	};

	std::unordered_map<void*, CacheBase*> _caches;

	LUCID_PREVENT_COPY(Resources);
	LUCID_PREVENT_ASSIGNMENT(Resources);
};

template<class T> inline std::shared_ptr<T> Resources::get(std::string const &key)
{
	Resources &resources = instance();
	std::unordered_map<void*, CacheBase*> &caches = resources._caches;

	ResourceCache<T> *cache = nullptr;

	std::unordered_map<void*, CacheBase*>::iterator iter = caches.find(Type<T>::ID());
	if (iter != caches.end())
	{
		cache = static_cast<ResourceCache<T>*>(iter->second);
	}
	else
	{
		cache = new ResourceCache<T>();
		caches[Type<T>::ID()] = cache;
	}

	return cache->get(key);
}

LUCID_GIGL_END
