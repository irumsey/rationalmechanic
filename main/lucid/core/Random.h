#pragma once

#include <random>
#include <lucid/core/Defines.h>
#include <lucid/core/Noncreatable.h>
#include <lucid/core/Types.h>

LUCID_CORE_BEGIN

/// random
///
/// utility class for generating random numbers
class random
{
public:
	static float32_t real(float32_t a, float32_t b)
	{
		return (b - a) * instance.real_distribution(instance.generator) + a;
	}

	static float32_t real()
	{
		return instance.real_distribution(instance.generator);
	}

	template<typename T> static T integer(T a, T b)
	{
		return T((instance.integer_distribution(instance.generator) % (b - a)) + a);
	}

	template<typename T> static T integer()
	{
		return T(instance.integer_distribution(instance.generator));
	}

private:

	struct Instance
	{
		Instance()
			: generator(device())
		{
		}

		std::random_device device;
		std::mt19937 generator;
		std::uniform_real_distribution<float32_t> real_distribution;
		std::uniform_int_distribution<uint32_t> integer_distribution;
	};

	static Instance instance;

	LUCID_PREVENT_CREATION(random);
};

LUCID_CORE_END