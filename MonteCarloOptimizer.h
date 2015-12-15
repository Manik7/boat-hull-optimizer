#ifndef MONTE_CARLO_OPTIMIZER_H
#define MONTE_CARLO_OPTIMIZER_H

#include <random>
#include "Optimizer.h"

template<typename OptimizableType>
class MonteCarloOptimizer : public Optimizer {
	
	OptimizableType model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	
	bool foundValidModel = false;
	double oldFitness = 0.0;
	
public:
	
#ifdef DETERMINISTIC_RUN
	MonteCarloOptimizer(OptimizableType model) : model(model), engine(0), 
#else
	MonteCarloOptimizer(OptimizableType model) : model(model), engine(std::mt19937(rd())),
#endif
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(0, 100)) {}
	
	void run(int steps = 1000000) {
		
		model.set_all_parameters(0);
		
		for(int i = 0; i<steps && !foundValidModel; ++i) {
			do_step();
		}
		
	}
	
	void do_step() {
		int index = indexDistribution(engine);
		int value = valueDistribution(engine);
		
		model.set_parameter(index, value);
		
		if(!foundValidModel) { // You haven't found a (valid) parameter set yet, i.e. one that meets the constraints
			if (model.satisfies_constraints()) { 
				foundValidModel = true;
				oldFitness = model.fitness();
			} // else just keep the change you made and do nothing further
		} 
		
		else { // You already had a valid model
			if (model.satisfies_constraints() && model.fitness() <= oldFitness) {
				oldFitness = model.fitness();
			} else {
				model.revert_last_change();
			}
		}
	}
};

#endif // MONTE_CARLO_OPTIMIZER_H
