#pragma once

#include <list>

namespace lucid {
namespace core {

	///	Stack
	///
	///	simple stack as a wrapper about std::list.
	///	exists just to consolidate top and pop.
	///	also to have a clear method.
	template<class T> class Stack final
	{
	public:
		Stack() = default;

		~Stack() = default;

		bool empty() const
		{
			return _internal.empty();
		}

		size_t size() const
		{
			return _internal.size();
		}

		void push(T value)
		{
			_internal.push_front(value);
		}

		T pop()
		{
			T value = _internal.front();
			_internal.pop_front();
			return value;
		}

		void clear()
		{
			return _internal.clear();
		}

	private:
		std::list<T> _internal;

	};

}	///	core
}	///	lucid