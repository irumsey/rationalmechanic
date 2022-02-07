#pragma once

#include <random>
#include <lucid/core/Noncreatable.h>
#include <lucid/core/Types.h>

namespace lucid {
namespace core {

	/// random
	///
	/// utility class for generating random numbers
	class random
	{
	public:
		static float32_t real(float32_t a, float32_t b)
		{
			return (b - a) * real_distribution(generator) + a;
		}

		static float32_t real()
		{
			return real_distribution(generator);
		}

		template<typename T> static T integer(T a, T b)
		{
			return T((integer_distribution(generator) % (b - a)) + a);
		}

		template<typename T> static T integer()
		{
			return T(integer_distribution(generator));
		}

	private:
		static std::mt19937 generator;
		static std::uniform_real_distribution<float32_t> real_distribution;
		static std::uniform_int_distribution<uint32_t> integer_distribution;

		LUCID_PREVENT_CREATION(random);
	};


}	// core
}	// lucid