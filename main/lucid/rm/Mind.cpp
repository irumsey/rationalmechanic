#include "Mind.h"
#include <lucid/core/Bits.h>

using random = ::lucid::core::random;

namespace lucid {
namespace rm {

	//
	//
	//

	Mind::Mind(size_t chromosomeLength, size_t genomeCount)
		: _chromosomeLength(chromosomeLength)
		, _genomeCount(genomeCount)
	{
		initialize(_population[0]);
		initialize(_population[1]);
	}

	void Mind::update(size_t throttle)
	{
		LUCID_PROFILE_SCOPE("rm::Mind::update");

		_fittest = 0;

		evaluate(_population[0], throttle);
		propagate(_population[1], _population[0]);
		advance();
	}

	void Mind::initialize(chromosome_t &chromosome) const
	{
		chromosome.resize(_chromosomeLength);

		std::transform(
			chromosome.begin(), chromosome.end(),
			chromosome.begin(),
			[](uint32_t /* ignored */) { return random::integer<uint32_t>(); }
		);
	}

	void Mind::evaluate(population_t &population, size_t throttle)
	{
		LUCID_VALIDATE(_genomeCount == population.size(), "population size mismatch");

		LUCID_PROFILE_SCOPE("rm::Mind::evaluate");

		float32_t accum = 0.f;
		for (size_t i = 0; i < _genomeCount; ++i)
		{
			genome_t &genome = population[i];

			float32_t fitness = measure(genome.chromosome, throttle);
			genome.fitness = std::make_pair(accum, accum + fitness);
			
			accum = genome.fitness.second;
		}

		// TBD: one more pass to normalize the fitness values.
		// this is not required if the roulette selection just ranges
		// from 0 to accum instead of 0 to 1.
		float32_t coeff = (0.f == accum) ? 0.f : 1.f / accum;
		auto normalize = [coeff](genome_t &genome) { genome.fitness.first *= coeff; genome.fitness.second *= coeff; };
		std::for_each(population.begin(), population.end(), normalize);
	}

	void Mind::propagate(population_t &next, population_t const &prev) const
	{
		LUCID_VALIDATE(_genomeCount == next.size(), "population size mismatch");
		LUCID_VALIDATE(_genomeCount == prev.size(), "population size mismatch");

		LUCID_PROFILE_SCOPE("rm::Mind::propagate");

		for (size_t i = 0; i < (_genomeCount - 1); i += 2)
		{
			size_t count = 0;
			size_t pos = random::integer<size_t>(0, _chromosomeLength);

			crossover(
				prev[select(prev)], prev[select(prev)],
				next[i + 0],
				next[i + 1],
				[pos, &count](auto /* ignored */, auto /* ignored */) { return ((count++) >= pos); }
			);

			mutate(next[i + 0].chromosome, 0.01f);
			mutate(next[i + 1].chromosome, 0.01f);
		}
	}

	float32_t Mind::measure(chromosome_t const &chromosome, size_t throttle)
	{
		LUCID_PROFILE_SCOPE("rm::Mind::measure");

		_machine.execute(chromosome, throttle);

		Graph const &graph = _machine.graph();
		size_t count = graph.nodeCount();

		float32_t fitness = 0.f;
		for (size_t i = 0; i < count; ++i)
			fitness = ((0 != graph.countUpstream(i)) && (0 != graph.countDownstream(i))) ? fitness + 1 : fitness;

		_fittest = std::max(_fittest, fitness);

		return fitness;
	}

	void Mind::mutate(chromosome_t &chromosome, float32_t rate) const
	{
		LUCID_PROFILE_SCOPE("rm::Mind::mutate");

		if (random::real() > rate)
			return;

		uint8_t *opaque = (uint8_t *)(&chromosome[0]);
		size_t mutationCount = random::integer<size_t>(0, 10);
		for (size_t i = 0; i < mutationCount; ++i)
		{
			size_t index = random::integer<size_t>(0, sizeof(gene_t) * _chromosomeLength);
			opaque[index] = random::integer<uint8_t>();
		}
	}

}	// rm
}	// lucid
