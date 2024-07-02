#include "Mind.h"
#include "Disassembler.h"
#include <lucid/core/Bits.h>
#include <sstream>
#include <fstream>

LUCID_RM_BEGIN

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

	_fittest = std::make_pair(0, 0.f);

	evaluate(_population[0], throttle);
	propagate(_population[1], _population[0]);
	advance();
}

void Mind::execute(Machine::Program const &program, size_t throttle)
{
	_machine.execute(program, throttle);
}

void Mind::initialize(chromosome_t &chromosome) const
{
	chromosome.resize(_chromosomeLength);

	std::transform(
		chromosome.begin(), chromosome.end(),
		chromosome.begin(),
		[](uint32_t /* ignored */) { return LUCID_CORE::random::integer<uint32_t>(); }
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
		_fittest = (fitness > _fittest.second) ? std::make_pair(i, fitness) : _fittest;

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

	next[0] = prev[_fittest.first];
	for (size_t i = 1; i < (_genomeCount - 2); i += 2)
	{
		size_t count = 0;
		size_t pos = LUCID_CORE::random::integer<size_t>(0, _chromosomeLength);

		genome_t const &parent0 = prev[select(prev)];
		genome_t const &parent1 = prev[select(prev)];

		crossover(
			parent0, parent1,
			next[i + 0],
			next[i + 1],
			[pos, &count](auto /* ignored */, auto /* ignored */) { return ((count++) >= pos); }
		);

		float32_t dFitness = (parent1.fitness.second - parent1.fitness.first) - (parent0.fitness.second - parent0.fitness.first);
		float32_t rate = (dFitness < 0.001f) ? 0.2f : 0.03f;

		mutate(next[i + 0].chromosome, rate);
		mutate(next[i + 1].chromosome, rate);
	}
}

float32_t Mind::measure(chromosome_t const &chromosome, size_t throttle)
{
	LUCID_PROFILE_SCOPE("rm::Mind::measure");

	execute(chromosome, throttle);

	Graph const &graph = _machine.graph();
	size_t count = graph.nodeCount();

	float32_t fitness = 0.f;
	for (size_t i = 0; i < count; ++i)
		fitness = ((0 != graph.countUpstream(i)) && (0 != graph.countDownstream(i))) ? fitness + 1 : fitness;

	return fitness;
}

void Mind::mutate(chromosome_t &chromosome, float32_t rate) const
{
	LUCID_PROFILE_SCOPE("rm::Mind::mutate");

	if (LUCID_CORE::random::real() > rate)
		return;

	uint8_t *opaque = (uint8_t *)(&chromosome[0]);
	size_t mutationCount = LUCID_CORE::random::integer<size_t>(1, 20);
	for (size_t i = 0; i < mutationCount; ++i)
	{
		size_t index = LUCID_CORE::random::integer<size_t>(0, sizeof(gene_t) * _chromosomeLength);
		opaque[index] = LUCID_CORE::random::integer<uint8_t>();
	}
}

LUCID_RM_END