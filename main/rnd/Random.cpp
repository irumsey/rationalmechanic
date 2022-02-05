#include "Random.h"

std::mt19937 Random::generator;
std::uniform_int_distribution<uint32_t> Random::distInteger;
std::uniform_real_distribution<float32_t> Random::distReal(0.f, 1.f);

float32_t Random::real(float32_t a, float32_t b)
{
	return (b - a) * distReal(generator) + a;
}

uint32_t Random::integer()
{
	return distInteger(generator);
}

uint32_t Random::integer(uint32_t a, uint32_t b)
{
	return distInteger(generator) % (b - a) + a;
}
