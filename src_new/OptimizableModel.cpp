#include "OptimizableModel.h"

OptimizableModel::OptimizableModel() : 
	indexDistribution(std::uniform_int_distribution<int>(0, numberOfGenes-1)),
	valueDistribution(std::uniform_int_distribution<int>(numberOfGenes, domainHi)),
	coinFlipDistribution(std::bernoulli_distribution(mutationRate)),
#ifdef DETERMINISTIC_RUN
	engine(std::mt19937(0))
#else
	engine(std::mt19937(rd()))
#endif
{
	/* TODO: The values are (and need to be) hard copied. 
	 * With an std::vector this would be less of a problem, 
	 * and the size could be verified via assertion as well. 
	 */
	
	for (int i = 0; i < numberOfGenes; ++i) { 
		genome[i].first = 0;
		genome[i].second = 0;
	}
	
	compute_fitness(); //TODO: remove this when you go for the lazy fitness calculation
}

OptimizableModel::OptimizableModel(std::pair<int, NumType> genes[]) : OptimizableModel()
{
	/* TODO: The values are (and need to be) hard copied. 
	 * With an std::vector this would be less of a problem, 
	 * and the size could be verified via assertion as well. 
	 */
	
	for (int i = 0; i < numberOfGenes; ++i) { 
		genome[i] = genes[i];		
	}
	
	compute_fitness(); //TODO: remove this when you go for the lazy fitness calculation
}

void OptimizableModel::crossover(OptimizableModel& partner, OptimizableModel& child)
{
	int crossover_point = indexDistribution(engine);
	
	int i = 0;
	for ( ; i < crossover_point && i < numberOfGenes; ++i) { //this one's genes are copied
		child.genome[i] = genome[i];
	}
	for ( ; i < numberOfGenes; ++i) { //partner's gene are copied
		child.genome[i] = partner.genome[i];
	}

// 	//NOTE: The following code packs this into a single loop with an if statement instead of split loops
// 	for (int i = 0; i < numberOfGenes; ++i) { //this one's genes are copied is selected
// 		if (i < crossover_point) {
// 			newGenome[i] = parameters[i];
// 		} else {
// 			newGenome[i] = partner.parameters[i];
// 		}
// 	}
	child.compute_fitness(); //TODO: remove this when you go for the lazy fitness calculation
}

void OptimizableModel::mutate()
{
	for (int i = 0; i < numberOfGenes; ++i) {
		if (coinFlipDistribution(engine)) { // choose if parameter is to mutate
			set_parameter(i, valueDistribution(engine)); // get a random value for the parameter
		}
	}
}