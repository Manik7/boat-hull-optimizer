#ifndef OPTIMIZABLE_MODEL_H
#define OPTIMIZABLE_MODEL_H

#include <utility>
#include <random>
#include <cassert>

struct OptimizableModel {
public:
 	typedef int NumType;
	using Optimizable_model = OptimizableModel;
	static constexpr int numberOfGenes = 15;
	static constexpr int domainLo = 0;
	static constexpr int domainHi = 100;
	static constexpr double mutation_standard_deviation = 0.3; // default = 0.3
	
protected:
	//<parameter domain (input values), parameter range (output, the real values)>
	std::pair<int,NumType> genome[numberOfGenes]; 
	
private:
	bool isFitnessUpdated = false;
	double m_fitness = 0.0;
public:
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 engine;
	static std::uniform_int_distribution<int> indexDistribution; //random index, i.e. gene
	static std::uniform_int_distribution<int> valueDistribution; //random domain value
	static std::normal_distribution<double> modifierDistribution; //mutation modifier
	static std::bernoulli_distribution coinFlipDistribution; //do you mutate or not?
	
	double fitness();
	
	OptimizableModel();
	OptimizableModel(std::mt19937 engine);
	
	/* Overwrites the 'child' object with a newly created Hull. Returns the 
	 * crossover point, i.e. the highest gene index taken from this parent. 
	 * A crossover-point of 1 would mean the first two genes came from this 
	 * parent, the rest from the partner.*/
	virtual int crossover(OptimizableModel& partner, OptimizableModel& child); 
	virtual void mutate();
	
	virtual void output() /*const*/=0;
	
	virtual inline void set_parameter(int gene_index, int domain_value) { // Sets the domain value & updates the fitness 
		genome[gene_index].first = domain_value;
		genome[gene_index].second = (get_range_max(gene_index) - get_range_min(gene_index)) * NumType(domain_value - domainLo) / (domainHi-domainLo) + get_range_min(gene_index);
		isFitnessUpdated = false;
	}
	
	inline NumType get_parameter(int index) const {
		return genome[index].first;
	}
	
protected:
	/*TODO: It might be worth going for a fitness() function which either gives you the stored value, 
	 * or updates the values first and then returns it if changes have been made. You'd need a simple
	 * flag such as 'isUpToDate' which you set to false every time set_parameter is called.
	 */
	virtual double compute_fitness()=0; //compute the fitness value
	
	/*TODO: as a performance improvement, you might be able to do with with 
	 * a template parameter to encourage to compiler to precompile one version 
	 * of the function for each parameter. */
	virtual NumType get_range_min(int index) const=0; // Get the min value for the range (i.e. real values) for the parameters
	virtual NumType get_range_max(int index) const=0;
};

#endif // OPTIMIZABLE_MODEL_H