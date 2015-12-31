#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>
#include <random>

template <typename T, int NUMBER_OF_PARAMETERS, T DOMAIN_LO = 0, T DOMAIN_HI = 100>
class OptimizableModel {

protected:
	std::pair<T,T> parameters[NUMBER_OF_PARAMETERS]; //parameter domain (input values), parameter range (output, the real values)
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	
public:

	OptimizableModel();
	
	void mutate();
	OptimizableModel crossover(OptimizableModel partner)=0; //TODO: Figure out the signature with the best performance
	virtual double fitness()=0;
	
	T set_parameter(int index, int domain_value)=0; // Sets the domain value, must also update the range value
	
	T get_parameter(int index);
	
protected:
	virtual T get_range_min()=0; // Get the min value for the range (i.e. real values) for the parameters
	virtual T get_range_max()=0;
};

#endif // OPTIMIZABLE_MODEL_H