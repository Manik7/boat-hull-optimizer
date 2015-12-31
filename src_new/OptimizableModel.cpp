#include "OptimizableModel.h"

#ifdef DETERMINISTIC_RUN
OptimizableModel::OptimizableModel() : engine(0), 
#else
OptimizableModel::OptimizableModel() : engine(std::mt19937(rd())),
#endif
	indexDistribution(std::uniform_int_distribution<int>(0, NUMBER_OF_PARAMETERS-1)), 
	valueDistribution(std::uniform_int_distribution<int>(-1, 1)) {}
		
		
T OptimizableModel::get_parameter(int index) {
	return parameters[index].first;
}