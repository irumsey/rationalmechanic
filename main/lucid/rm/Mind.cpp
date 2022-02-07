#include "Mind.h"
#include <lucid/core/Random.h>

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
		float32_t fitsum = evaluate(_population[0], throttle);
		generate(_population[1], _population[0], fitsum);
		_population[0].swap(_population[1]);
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

	float32_t Mind::evaluate(population_t &population, size_t throttle)
	{
		LUCID_VALIDATE(_genomeCount == population.size(), "population size mismatch");

		float32_t accum = 0.f;
		for (size_t i = 0; i < _genomeCount; ++i)
		{
			genome_t &genome = population[i];

			float32_t fitness = measure(genome.chromosome, throttle);
			genome.fitness = std::make_pair(accum, accum + fitness);
			
			accum = genome.fitness.second;
		}

		return accum;
	}

	void Mind::generate(population_t &next, population_t const &prev, float32_t fitsum) const
	{
		LUCID_VALIDATE(_genomeCount == next.size(), "population size mismatch");
		LUCID_VALIDATE(_genomeCount == prev.size(), "population size mismatch");

		for (size_t i = 0; i < (_genomeCount - 1); i += 2)
		{
			size_t count = 0;
			size_t pos = random::integer<size_t>(0, _chromosomeLength);

			crossover(
				prev[select(prev, random::real(0.f, fitsum))], prev[select(prev, random::real(0.f, fitsum))],
				next[i + 0], next[i + 1],
				[pos, &count](auto, auto) { return ((count++) >= pos); }
			);

			mutate(next[i + 0].chromosome, 0.01f);
			mutate(next[i + 1].chromosome, 0.01f);
		}
	}

	float32_t Mind::measure(chromosome_t const &chromosome, size_t throttle)
	{
		_machine.execute(chromosome, throttle);

		Graph const &graph = _machine.graph();
		size_t count = graph.nodeCount();

		float32_t fitness = 0.f;
		for (size_t i = 0; i < count; ++i)
			fitness = ((0 != graph.countUpstream(i)) && (0 != graph.countDownstream(i))) ? fitness + 1 : fitness;

		return fitness;
	}

	void Mind::mutate(chromosome_t &chromosome, float32_t rate) const
	{
		auto func = [rate](uint32_t gene) { return (random::real() > rate) ? gene : random::integer<uint32_t>(); };
		std::transform(chromosome.begin(), chromosome.end(), chromosome.begin(), func);
	}

}	// rm
}	// lucid
