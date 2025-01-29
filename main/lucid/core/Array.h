#pragma once

#include <unordered_map>

#include <lucid/core/Defines.h>
#include <lucid/core/Meta.h>

///
///
///
LUCID_CORE_BEGIN

///	Array
///
/// Note: or just use std::array<>
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

	Array(Array const &rhs)
	{
		meta<T>::copy<LAST>(_data, rhs._data);
	}

	Array(T const &value)
	{
		fill(value);
	}

	~Array() = default;

	size_t count() const
	{
		return N;
	}

	self_t &operator=(self_t const &rhs)
	{
		if (this == &rhs)
			return *this;

		meta<T>::copy<LAST>(_data, rhs._data);

		return *this;
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

LUCID_CORE_END