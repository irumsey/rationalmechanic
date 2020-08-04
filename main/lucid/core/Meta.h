#pragma once

#include <lucid/core/Noncreatable.h>

///
///
///
namespace lucid {
namespace core {

	///	meta
	///
	///	some straightforward meta programming concepts.
	template<typename T> struct meta
	{

		template<int index> static void fill(T *data, T const &value)
		{
			data[index] = value;
			fill<index - 1>(data, value);
		}

		template<> static void fill<0>(T *data, T const &value)
		{
			data[0] = value;
		}

		template<int index> static void copy(T *data, T const *rhs)
		{
			data[index] = rhs[index];
			copy<index - 1>(data, rhs);
		}

		template<> static void copy<0>(T *data, T const *rhs)
		{
			data[0] = rhs[0];
		}

		LUCID_PREVENT_CREATION(meta);

	};

}	///	core
}	///	lucid