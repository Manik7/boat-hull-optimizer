#include "OptimizableModel.h"

OptimizableModel::OptimizableModel() : 
	indexDistribution(std::uniform_int_distribution<int>(0, NUMBER_OF_GENES-1)),
	valueDistribution(std::uniform_int_distribution<int>(DOMAIN_LO, DOMAIN_HI)),
	coinFlipDistribution(std::bernoulli_distribution(MUTATION_RATE)),
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
	
	for (int i = 0; i < NUMBER_OF_GENES; ++i) { 
		genome[i].first = 0;
		genome[i].second = 0;
	}
	
	compute_fitness(); //TODO: remove this when you go for the lazy fitness calculation
}

OptimizableModel::OptimizableModel(std::pair<int, T> genes[]) : OptimizableModel()
{
	/* TODO: The values are (and need to be) hard copied. 
	 * With an std::vector this would be less of a problem, 
	 * and the size could be verified via assertion as well. 
	 */
	
	for (int i = 0; i < NUMBER_OF_GENES; ++i) { 
		genome[i] = genes[i];		
	}
	
	compute_fitness(); //TODO: remove this when you go for the lazy fitness calculation
}

OptimizableModel OptimizableModel::crossover(OptimizableModel& partner)
{
	std::pair<T,T> newGenome[NUMBER_OF_GENES];
	int crossover_point = OptimizableModel.indexDistribution(engine);
	
	int i = 0;
	for ( ; i < crossover_point && i < NUMBER_OF_GENES; ++i) { //this one's genes are copied
		newGenome[i] = genome[i];
	}
	for ( ; i < NUMBER_OF_GENES; ++i) { //partner's gene are copied
		newGenome[i] = partner.parameters[i];
	}

// 	//NOTE: The following code packs this into a single loop with an if statement instead of split loops
// 	for (int i = 0; i < NUMBER_OF_GENES; ++i) { //this one's genes are copied is selected
// 		if (i < crossover_point) {
// 			newGenome[i] = parameters[i];
// 		} else {
// 			newGenome[i] = partner.parameters[i];
// 		}
// 	}
	
	return OptimizableModel(newGenome);
}

void OptimizableModel::mutate()
{
	for (int i = 0; i < NUMBER_OF_GENES; ++i) {
		if (coinFlipDistribution(engine)) { // choose if parameter is to mutate
			set_parameter(i, OptimizableModel.valueDistribution(engine)); // get a random value for the parameter
		}
	}
}

T OptimizableModel::get_parameter(int index) {
	return genome[index].first;
}

void OptimizableModel::set_parameter(int index, int domain_value)
{
	genome[index].first = domain_value;
	genome[index].second = (get_range_max()-get_range_min()) * T(domain_value - DOMAIN_LO) / (DOMAIN_HI-DOMAIN_LO) + get_range_min();
	compute_fitness();
}
