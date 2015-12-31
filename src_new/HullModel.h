#ifndef HULL_MODEL_H
#define HULL_MODEL_H

#include "OptimizableModel.h"

template <typename T, int NUMBER_OF_PARAMETERS, T DOMAIN_LO = 0, T DOMAIN_HI = 100>
class HullModel : public OptimizableModel<T, NUMBER_OF_PARAMETERS, DOMAIN_LO, DOMAIN_HI> {
	
public:	
	double fitness();

private:
	T map_parameter(int domain_value);
};

#endif //HULL_MODEL_H