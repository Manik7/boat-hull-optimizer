#include "OptimizableModel.h"

OptimizableModel::OptimizableModel(std::pair<int, T> genome[])
{
	/* TODO: The values are (and need to be) hard copied. 
	 * With an std::vector this would be less of a problem, 
	 * and the size could be verified via assertion as well. 
	 */
	
	for (int i = 0; i < NUMBER_OF_PARAMETERS; ++i) { 
		parameters[i] = genome[i];		
	}
	
	compute_fitness();
}

OptimizableModel OptimizableModel::crossover(OptimizableModel& partner)
{
	std::pair<T,T> newGenome[NUMBER_OF_PARAMETERS];
	int crossover_point = OptimizableModel.indexDistribution(engine);
	
	int i = 0;
	for ( ; i < NUMBER_OF_PARAMETERS && i < crossover_point; ++i) { //this one's genes are copied
		newGenome[i] = parameters[i];
	}
	for ( ; i < NUMBER_OF_PARAMETERS; ++i) { //partner's gene are copied
		newGenome[i] = partner.parameters[i];
	}
	
// 	for (int i = 0; i < NUMBER_OF_PARAMETERS; ++i) { //this one's genes are copied is selected
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
	for (int i = 0; i < NUMBER_OF_PARAMETERS; ++i) {
		if (coinFlipDistribution(engine)) { // choose if parameter is to mutate
			set_parameter(i, OptimizableModel.valueDistribution(engine)); // get a random value for the parameter
		}
	}
}

T OptimizableModel::get_parameter(int index) {
	return parameters[index].first;
}

void OptimizableModel::set_parameter(int index, int domain_value)
{
	parameters[index].first = domain_value;
	parameters[index].second = (get_range_max()-get_range_min()) * T(domain_value - DOMAIN_LO) / (DOMAIN_HI-DOMAIN_LO) + get_range_min();
	compute_fitness();
}
