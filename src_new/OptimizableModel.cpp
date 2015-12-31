#include "OptimizableModel.h"

OptimizableModel::OptimizableModel(std::pair< T, T > genome[])
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


void OptimizableModel::mutate()
{
	int index = OptimizableModel.indexDistribution(engine);	// choose parameter to modify
	int value = OptimizableModel.valueDistribution(engine);	// get a random value for the parameter
	
	set_parameter(index, value);
}

OptimizableModel OptimizableModel::crossover(OptimizableModel& partner)
{
	std::pair<T,T> newGenome[NUMBER_OF_PARAMETERS];
	
	for (int i = 0; i<NUMBER_OF_PARAMETERS; ++i) {
		if(coinFlipDistribution(engine)) { //this one's gene is selected
			newGenome[i] = parameters[i];
			
		} else { //partner's gene is selected
			newGenome[i] = partner.parameters[i];
		}
	}
	
	return OptimizableModel(newGenome);
}


T OptimizableModel::get_parameter(int index) {
	return parameters[index].first;
}

void OptimizableModel::set_parameter(int index, int domain_value)
{
	parameters[index].first = domain_value;
	parameters[index].second = (get_range_max()-get_range_min()) * (domain_value - DOMAIN_LO) / (DOMAIN_HI-DOMAIN_LO) + get_range_min();
	compute_fitness();
}
