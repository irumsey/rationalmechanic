#pragma once

#include <algorithm>
#include <vector>
#include <lucid/core/Noncopyable.h>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>
#include <lucid/rm/GA.h>
#include <lucid/rm/Machine.h>

namespace lucid {
namespace rm {

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

		void update(size_t throttle);

	private:
		typedef std::pair<float32_t, float32_t> measure_t;
		typedef Machine::Program chromosome_t;
		typedef Genome<uint32_t, measure_t, Machine::Program> genome_t;
		typedef std::vector<genome_t> population_t;

		size_t _chromosomeLength = 0;
		size_t _genomeCount = 0;

		population_t _population[2];	// previous/next generation

		Machine _machine;

		void initialize(chromosome_t &chromosome) const;

		void initialize(genome_t &genome) const;

		void initialize(population_t &population) const;

		float32_t evaluate(population_t &population, size_t throttle);

		void generate(population_t &next, population_t const &prev, float32_t fitsum) const;

		float32_t measure(chromosome_t const &chromosome, size_t throttle);

		size_t select(population_t const &population, float32_t position) const;

		void mutate(chromosome_t &chromosome, float32_t rate) const;

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

	inline void Mind::initialize(genome_t &genome) const
	{
		initialize(genome.chromosome);
	}

	inline void Mind::initialize(population_t &population) const
	{
		population.resize(_genomeCount);
		std::for_each(population.begin(), population.end(), [this](auto &genome) { initialize(genome); });
	}

	inline size_t Mind::select(population_t const &population, float32_t position) const
	{
		LUCID_VALIDATE(_genomeCount == population.size(), "population size mismatch");

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

}	// rm
}	// lucid
