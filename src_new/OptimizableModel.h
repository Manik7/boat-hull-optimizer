#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>
#include <random>

template <typename T, int NUMBER_OF_PARAMETERS, int DOMAIN_LO = 0, int DOMAIN_HI = 100>
class OptimizableModel {

protected:
	std::pair<int,T> parameters[NUMBER_OF_PARAMETERS]; //parameter domain (input values), parameter range (output, the real values)
	
public:

	static std::random_device rd; // obtain a random number from hardware
	
#ifdef DETERMINISTIC_RUN
	static std::mt19937 engine(0);
#else
	static std::mt19937 engine(rd());
#endif
	static std::uniform_int_distribution<int> indexDistribution<int>(0, NUMBER_OF_PARAMETERS-1);
	static std::uniform_int_distribution<int> valueDistribution<int>(DOMAIN_LO, DOMAIN_HI);
	static std::bernoulli_distribution coinFlipDistribution(0.5);

	double fitness = 0.0;
	
	OptimizableModel(std::pair< int, T > genome[]);
	
	void mutate();
	OptimizableModel crossover(OptimizableModel& partner); //TODO: Figure out the signature with the best performance
	
	inline void set_parameter(int index, int domain_value); // Sets the domain value & updates the fitness
	inline T get_parameter(int index);
	
protected:
	virtual void compute_fitness()=0; //compute and set the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with 
	 * a template parameter to encourage to compiler to precompile one version 
	 * of the function for each parameter. */
	virtual inline T get_range_min(int index)=0; // Get the min value for the range (i.e. real values) for the parameters
	virtual inline T get_range_max(int index)=0;
};

#endif // OPTIMIZABLE_MODEL_H