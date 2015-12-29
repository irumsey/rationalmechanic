#pragma once

#include <algorithm>
#include <vector>

namespace lucid {
namespace core {

	///	Queue
	///
	///	same as stl priority_queue.
	///	however, this allows me to experiment with the internals.
	///	also, it contains a clear() method.
	template<typename T, typename Pred> class Queue final
	{
	public:
		Queue() = default;

		~Queue() = default;

		bool empty() const
		{
			return _heap.empty();
		}

		uint32_t count() const
		{
			return _heap.size();
		}

		void push(T const &value)
		{
			_heap.push_back(value);
			std::push_heap(_heap.begin(), _heap.end(), _pred);
		}

		void pop()
		{
			std::pop_heap(_heap.begin(), _heap.end(), _pred);
			_heap.pop_back();
		}

		T const &top() const
		{
			return _heap.front();
		}

		void clear()
		{
			_heap.clear();
		}

	private:
		Pred _pred;
		std::vector<T> _heap;

	};

}	///	core
}	///	lucid