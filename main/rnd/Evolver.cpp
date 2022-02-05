#include "Evolver.h"

void Evolver::initialize(size_t sizePopulation)
{
	population[0].resize(sizePopulation);
	population[1].resize(sizePopulation);

	for (size_t i = 0; i < sizePopulation; ++i)
		randomProgram(population[0][i].program);
}

void Evolver::evolve(size_t execLimit)
{
	size_t popSize = population[0].size();

	float32_t accum = 0.f;
	float32_t fittest = 0.f;
	for (size_t i = 0; i < popSize; ++i)
	{
		machine.execute(population[0][i].program, execLimit);
		float32_t fitness = measureFitness(population[0][i]);

		population[0][i].fitness = std::make_pair(accum, accum + fitness);
		accum = population[0][i].fitness.second;

		fittest = std::max(fittest, fitness);
	}

	for (size_t i = 0; i < (popSize - 1); i += 2)
	{
		size_t leftIndex = select(population[0], Random::real(0, accum));
		size_t rightIndex = select(population[0], Random::real(0, accum));

		crossover(
			population[1][i + 0].program,
			population[1][i + 1].program,
			population[0][leftIndex].program,
			population[0][rightIndex].program
		);

		mutate(population[1][i + 0].program, 0.005f);
		mutate(population[1][i + 1].program, 0.005f);
	}

	// population[0] is always the current population after each generation
	population[0].swap(population[1]);
}

float32_t Evolver::measureFitness(Chromosome const &chomosome) const
{
	Graph const &graph = machine.theGraph();
	size_t count = graph.nodeCount();
	
	float32_t fitness = 0.f;
	for (size_t i = 0; i < count; ++i)
	{
		fitness  = ((0 != graph.countUpstream(i)) && (0 != graph.countDownstream(i))) ? fitness + 1 : fitness;
	}

	return fitness;
}
