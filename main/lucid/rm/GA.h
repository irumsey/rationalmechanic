#pragma once

#include <vector>
#include <lucid/core/Types.h>
#include <lucid/core/Error.h>

namespace lucid {
namespace rm {

	///	Genome
	///
	///	
	template<class Gene, class Measure = float32_t, class Chromosome = std::vector<Gene> > struct Genome
	{
		typedef Gene gene_t;
		typedef Measure measure_t;
		typedef Chromosome chromosome_t;
		typedef Genome<Gene, Measure, Chromosome> self_t;

		Chromosome chromosome;
		Measure fitness = Measure();
	};

	///	crossover_copy
	///
	///	generic crossover operation in the "stl style"
	template<class InIt1, class InIt2, class OutIt1, class OutIt2, class Pred> inline void crossover_copy
	(
		InIt1 src1, InIt1 const &&end1,
		InIt2 src2,
		OutIt1 dst1,
		OutIt2 dst2,
		Pred pred
	)
	{
		LUCID_VALIDATE(src1 <= end1, "invalid source range for crossover_copy");

		for (; src1 != end1; ++src1, ++src2, ++dst1, ++dst2)
		{
			if (pred(src1, src2))
			{
				*dst1 = *src2;
				*dst2 = *src1;
			}
			else
			{
				*dst1 = *src1;
				*dst2 = *src2;
			}
		}
	}

	///	crossover
	///
	///	helper for chromosome specific crossover
	template<class Gene, class Measure, class Chromosome, class Pred> inline void crossover(
		Genome<Gene, Measure, Chromosome> const &src1, Genome<Gene, Measure, Chromosome> const &src2,
		Genome<Gene, Measure, Chromosome> &dst1, Genome<Gene, Measure, Chromosome> &dst2,
		Pred pred
	)
	{
		LUCID_VALIDATE(src1.chromosome.size() == src2.chromosome.size(), "chromosome size mismatch");
		LUCID_VALIDATE(src1.chromosome.size() == dst1.chromosome.size(), "chromosome size mismatch");
		LUCID_VALIDATE(src1.chromosome.size() == dst2.chromosome.size(), "chromosome size mismatch");

		crossover_copy(
			src1.chromosome.begin(), src1.chromosome.end(),
			src2.chromosome.begin(),
			dst1.chromosome.begin(),
			dst2.chromosome.begin(),
			pred
		);
	}

}	// rm
}	// lucid
