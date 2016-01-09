#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>
#include <random>
#include <cassert>

// template <typename NumberType, int NUMBER_OF_GENES, int DOMAIN_LO = 0, int DOMAIN_HI = 100>
struct OptimizableModel {
public:
 	typedef int NumType;
	using Optimizable_model = OptimizableModel; //<NumberType, NUMBER_OF_GENES, DOMAIN_LO, DOMAIN_HI>;
	static constexpr int numberOfGenes = 15;
	static constexpr int domainLo = 0;
	static constexpr int domainHi = 100;
	static constexpr double mutationRate = 0.01;
	
protected:
	std::pair<int,NumType> genome[numberOfGenes]; //parameter domain (input values), parameter range (output, the real values)
	
private:
	bool isFitnessUpdated = false;
	double m_fitness = 0.0;
public:
	std::random_device rd; // obtain a random number from hardware //TODO: make static
	std::mt19937 engine; //TODO: make static
	std::uniform_int_distribution<int> indexDistribution; //TODO: make static
	std::uniform_int_distribution<int> valueDistribution; //TODO: make static
	std::bernoulli_distribution coinFlipDistribution; //TODO: make static
	
	double fitness();
	
	OptimizableModel();
	OptimizableModel(std::pair< int, NumType > genes[]);
	
	void crossover(OptimizableModel& partner, OptimizableModel& child); //TODO: Figure out the signature with the best performance
	void mutate();
	
	virtual void output() const=0;
	
	// TODO: set_parameter always updates the fitness, meaning many useless calculations if multiple parameters are changed 'at once'
	inline void set_parameter(int index, int domain_value) { // Sets the domain value & updates the fitness 
		genome[index].first = domain_value;
		genome[index].second = (get_range_max(index) - get_range_min(index)) * NumType(domain_value - domainLo) / (domainHi-domainLo) + get_range_min(index);
	}
	
	inline NumType get_parameter(int index) const {
		return genome[index].first;
	}
	
protected:
	/*TODO: It might be worth going for a fitness() function which either gives you the stored value, 
	 * or updates the values first and then returns it if changes have been made. You'd need a simple
	 * flag such as 'isUpToDate' which you set to false every time set_parameter is called.
	 */
	virtual double compute_fitness() const=0; //compute the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with 
	 * a template parameter to encourage to compiler to precompile one version 
	 * of the function for each parameter. */
	virtual NumType get_range_min(int index) const=0; // Get the min value for the range (i.e. real values) for the parameters
	virtual NumType get_range_max(int index) const=0;
};

#endif // OPTIMIZABLE_MODEL_H