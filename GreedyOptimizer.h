#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

// #include <iostream>
#include <random>
#include <cassert>

#include "Optimizer.h"
#include "Optimizable.h"

template<typename OptimizableType>
class GreedyOptimizer : public Optimizer {

	OptimizableType model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::uniform_int_distribution<int> diceRollDistribution;
	const double epsilon;

public:
#ifdef DETERMINISTIC_RUN
	GreedyOptimizer(OptimizableType& model) : model(model), engine(0), 
#else
	GreedyOptimizer(OptimizableType& model) : model(model), engine(std::mt19937(rd())),
#endif
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-1, 1)), 
		diceRollDistribution(std::uniform_int_distribution<int>(0,100)),
		epsilon(0.00001) { }
		
	void run(int steps = 2) {
		assert(steps > 0);
		//TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing

		for (int i = 0; i < steps; ++i) {
			do_step();
		}
		model.output();
	}
	
	void do_step() {
	
		double oldFitness = model.fitness();		// store old fitness
		int index = indexDistribution(engine);		// choose parameter to modify
		int oldValue = model.get_parameter(index); 	// store old parameter value	
		int modifier = valueDistribution(engine);	// get random value as a modifier for the parameter
						
		// modify the parameter
		if (oldValue + modifier > model.maxValue) { //resulting value too big
			model.set_parameter(index, model.maxValue);
		} 
		else if (oldValue + modifier < model.minValue) { //too small
			model.set_parameter(index, model.minValue);
		} 
		else { //can be changed freely
			model.set_parameter(index, oldValue + modifier);
		}
		
		// decide whether or not to keep the new value
		if(model.satisfies_constraints() && ( (model.fitness() < oldFitness-epsilon) || win_dice_roll(80))) { //TODO: adjust dice roll probability
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			model.revert_last_change(); //revert
		}
	}
private:
	bool win_dice_roll(int odds = 50)	{
		assert(odds >= 0 && odds <= 100);
		if (diceRollDistribution(engine) < odds) return true;
		else return false;
	}
};

#endif //GREEDY_OPTIMIZER_H
