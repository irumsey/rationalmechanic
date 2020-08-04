#pragma once

#include <lucid/core/Meta.h>

///
///
///
namespace lucid {
namespace core {

	///	Array
	///
	///	Fixed size array.
	template<size_t N, typename T> class Array
	{
	public:
		enum { COUNT = N };
		enum { LAST = COUNT - 1 };

		typedef typename T  value_t;
		typedef typename Array<COUNT, value_t> self_t;

		Array()
		{
			fill(T());
		}

		Array(Array const &) = default;

		virtual ~Array() = default;

		size_t count() const
		{
			return N;
		}

		value_t const &operator[](size_t index) const
		{
			return at(index);
		}

		value_t &operator[](size_t index)
		{
			return at(index);
		}

		value_t const &at(size_t index) const
		{
			return _data[index];
		}

		value_t &at(size_t index)
		{
			return _data[index];
		}

		void fill(T const &value)
		{
			meta<T>::fill<LAST>(_data, value);
		}

		T const *ptr() const
		{
			return _data;
		}

	private:
		T _data[N];

	};

}	///	core
}	/// lucid
