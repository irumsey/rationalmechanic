#pragma once

#include <cstdlib>
#include <algorithm>
#include <vector>
#include <lucid/core/Types.h>
#include <rnd/Random.h>
#include <rnd/Machine.h>

#ifdef min
#	undef min
#	undef max
#endif

class Evolver
{
public:
	typedef Machine::Program Program;

	struct Chromosome
	{
		std::pair<float32_t, float32_t> fitness;
		Program program;
	};

	typedef std::vector<Chromosome> population_t;

	Evolver() = default;

	virtual ~Evolver() = default;

	void initialize(size_t sizePopulation);

	void evolve(size_t execLimit);

private:
	size_t const sizeChromosome = 200;

	Machine machine;
	population_t population[2];

	void randomProgram(Program &program) const;

	void mutate(Program &program, float32_t rate) const;

	void crossover(Program &child, Program const &lParent, Program const &rParent) const;

	void crossover(Program &lChild, Program &rChild, Program const &lParent, Program const &rParent) const;

	float32_t measureFitness(Chromosome const &chromosome) const;

	size_t select(population_t const &population, float32_t position) const;
};

inline void Evolver::randomProgram(Program &program) const
{
	program.resize(sizeChromosome);
	for (size_t i = 0; i < sizeChromosome; ++i)
		program[i] = Random::integer();
}

inline void Evolver::mutate(Program &program, float32_t rate) const
{
	for (size_t i = 0; i < program.size(); ++i)
		program[i] = (Random::real(0.f, 1.f) > rate) ? program[i] : Random::integer();
}

inline void Evolver::crossover(Program &child, Program const &lParent, Program const &rParent) const
{
	size_t count = std::min(lParent.size(), rParent.size());

	size_t  lpos = Random::integer(0, count);
	size_t  rpos = lpos;

	child.resize(lpos + (rParent.size() - rpos));

	std::copy(lParent.begin(), lParent.begin() + lpos, child.begin());
	std::copy(rParent.begin() + rpos, rParent.end(), child.begin() + lpos);
}

inline void Evolver::crossover(Program &lChild, Program &rChild, Program const &lParent, Program const &rParent) const
{
	crossover(lChild, lParent, rParent);
	crossover(rChild, rParent, lParent);
}

inline size_t Evolver::select(population_t const &population, float32_t position) const
{
	size_t upper = population.size();
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

	// shut the compiler up (should never get here)
	return -1;
}
