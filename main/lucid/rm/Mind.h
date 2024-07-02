#pragma once

#include <algorithm>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/core/Random.h>
#include <lucid/core/Profiler.h>
#include <lucid/rm/Defines.h>
#include <lucid/rm/GA.h>
#include <lucid/rm/Machine.h>

LUCID_RM_BEGIN

///
///
///
class Mind
{
public:
	Mind(size_t chromosomeLength, size_t genomeCount);

	virtual ~Mind() = default;

	Graph &graph();

	Graph const &graph() const;

	Machine::Program const &chromosome() const;

	void update(size_t throttle);

	void execute(Machine::Program const &program, size_t throttle);

private:
	typedef std::pair<float32_t, float32_t> measure_t;
	typedef uint32_t gene_t;
	typedef Machine::Program chromosome_t;
	typedef Genome<gene_t, measure_t, chromosome_t> genome_t;
	typedef std::vector<genome_t> population_t;

	size_t _chromosomeLength = 0;
	size_t _genomeCount = 0;

	population_t _population[2];
	std::pair<size_t, float32_t> _fittest;

	Machine _machine;

	void initialize(chromosome_t &chromosome) const;

	void initialize(genome_t &genome) const;

	void initialize(population_t &population) const;

	void evaluate(population_t &population, size_t throttle);

	void propagate(population_t &next, population_t const &prev) const;

	float32_t measure(chromosome_t const &chromosome, size_t throttle);

	size_t select(population_t const &population) const;

	void mutate(chromosome_t &chromosome, float32_t rate) const;

	void advance();

	LUCID_PREVENT_COPY(Mind);
	LUCID_PREVENT_ASSIGNMENT(Mind);
};

inline Graph &Mind::graph()
{
	return _machine.graph();
}

inline Graph const &Mind::graph() const
{
	return _machine.graph();
}

inline Machine::Program const &Mind::chromosome() const
{
	return _population[0][_fittest.first].chromosome;
}

inline void Mind::initialize(genome_t &genome) const
{
	initialize(genome.chromosome);
}

inline void Mind::initialize(population_t &population) const
{
	population.resize(_genomeCount);
	std::for_each(population.begin(), population.end(), [this](auto &genome) { initialize(genome); });
}

inline size_t Mind::select(population_t const &population) const
{
	LUCID_VALIDATE(_genomeCount == population.size(), "population size mismatch");

	LUCID_PROFILE_SCOPE("rm::Mind::select");

	float32_t position = ::lucid::core::random::real();

	size_t upper = _genomeCount;
	size_t lower = 0;

	while (true)
	{
		size_t middle = (upper + lower) >> 1;

		auto const &fitness = population[middle].fitness;
		if ((fitness.first <= position) && (position < fitness.second))
			return middle;

		if (position < fitness.first)
			upper = middle;
		else
			lower = middle;
	}

	// shut the compiler up (can never get here)
	return -1;
}

inline void Mind::advance()
{
	LUCID_PROFILE_SCOPE("rm::Mind::advance");
	_population[0].swap(_population[1]);
}

LUCID_RM_END