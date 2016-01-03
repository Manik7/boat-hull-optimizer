#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>
#include <random>

template <typename NumberType, int NUMBER_OF_GENES, int DOMAIN_LO = 0, int DOMAIN_HI = 100>
struct OptimizableModel {
public:
 	typedef NumberType T;
	static constexpr int numberOfGenes = NUMBER_OF_GENES;
	static constexpr int domainLo = DOMAIN_LO;
	static constexpr int domainHi = DOMAIN_HI;
	
protected:
	std::pair<int,T> genome[NUMBER_OF_GENES]; //parameter domain (input values), parameter range (output, the real values)
	
public:
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 engine;
	static std::uniform_int_distribution<int> indexDistribution;
	static std::uniform_int_distribution<int> valueDistribution;
	static std::bernoulli_distribution coinFlipDistribution;
	static constexpr double MUTATION_RATE = 0.01;
	
	double fitness = 0.0;
	
	OptimizableModel();
	OptimizableModel(std::pair< int, T > genome[]);
	
	OptimizableModel crossover(OptimizableModel& partner); //TODO: Figure out the signature with the best performance
	void mutate();
	
	virtual void output()=0;
	
	// TODO: set_parameter always updates the fitness, meaning many useless calculations if multiple parameters are changed 'at once'
	inline void set_parameter(int index, int domain_value); // Sets the domain value & updates the fitness 
	inline T get_parameter(int index);
	
protected:
	/*TODO: It might be worth going for a fitness() function which either gives you the stored value, 
	 * or updates the values first and then returns it if changes have been made. You'd need a simple
	 * flag such as 'isUpToDate' which you set to false every time set_parameter is called.
	 */
	virtual void compute_fitness()=0; //compute and set the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with 
	 * a template parameter to encourage to compiler to precompile one version 
	 * of the function for each parameter. */
	virtual inline T get_range_min(int index); // Get the min value for the range (i.e. real values) for the parameters
	virtual inline T get_range_max(int index);
};

#endif // OPTIMIZABLE_MODEL_H