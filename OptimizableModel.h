#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>

template <typename T, int NUMBER_OF_PARAMETERS>
class OptimizableModel {

protected:
	std::pair<T,T> parameters[NUMBER_OF_PARAMETERS]; //parameter domain (input values), parameter range (output, the real values)

public:
	virtual void mutate()=0;
	virtual OptimizableModel crossover(OptimizableModel)=0; //TODO: Figure out the signature with the best performance
	virtual double fitness()=0;
	
	T get_parameter(int index);
	T set_parameter(int index, int domain_value); // Sets the domain value, must also update the range value
	
	
};

#endif // OPTIMIZABLE_MODEL_H