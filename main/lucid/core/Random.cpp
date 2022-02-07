#include "Random.h"

namespace lucid {
namespace core {

	std::mt19937 random::generator;
	std::uniform_real_distribution<float32_t> random::real_distribution(0.f, 1.f);
	std::uniform_int_distribution<uint32_t> random::integer_distribution;

}	// core
}	// lucid