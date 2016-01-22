#ifndef GENETIC_OPTIMIZER_H
#define GENETIC_OPTIMIZER_H

#include "Optimizer.h"
#include "HullModel.h"

// template <typename OptimizableModel >
class GeneticOptimizer : public Optimizer {

	constexpr int population_size = 1000;
	HullModel population[population_size];
	
	void run(int steps = 5);

	void do_step() {
		
		/*   I. Use stochastic acceptance to select a set of parents
		 *  II. Iterate over this chosen set of parents (check their birthdays as you go 
		 * 	along, to be sure this isn't an infant), and then select a random member
		 * 	of the population to overwrite with teh offspring of these two parents.
		 * 	If a parent has indeed been overwritten by the time it would be his turn 
		 * 	for crossover, well then that's too bad. Skip the newborn and take the 
		 * 	next member of the set of parents.
		 * III. Iterate over the entire population adding all the fitness values up for 
		 * 	a total, needed for the next run.
		 * 
		 * */
		
	}

};

#endif //GENETIC_OPTIMIZER_H