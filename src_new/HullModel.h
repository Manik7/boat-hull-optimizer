#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include "OptimizableModel.h"

template <typename T, int NUMBER_OF_PARAMETERS, int DOMAIN_LO = 0, int DOMAIN_HI = 100, double MUTATION_RATE = 0.01>
class HullModel : public OptimizableModel<T, NUMBER_OF_PARAMETERS, DOMAIN_LO, DOMAIN_HI, MUTATION_RATE> {
	
protected:
	void compute_fitness(); //compute and set the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with 
		* a template parameter to encourage to compiler to precompile one version 
		* of the function for each parameter. */
	inline T get_range_min(int index); // Get the min value for the range (i.e. real values) for the parameters
	inline T get_range_max(int index);
};

#endif //HULL_MODEL_H