#pragma once

#include <cinttypes>
#include <random>
#include <lucid/core/Types.h>

class Random
{
public:
	Random() = default;

	virtual ~Random() = default;

	static float32_t real(float32_t a, float32_t b);

	static uint32_t integer();

	static uint32_t integer( uint32_t a,  uint32_t b);

private:
	static std::mt19937 generator;
	static std::uniform_int_distribution<uint32_t> distInteger;
	static std::uniform_real_distribution<float32_t> distReal;

};

