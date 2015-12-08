#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

#include <random>
#include "Optimizable.h"

template<typename OptimizableType>
class GreedyOptimizer {

	OptimizableType model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::uniform_int_distribution<int> diceRollDistribution;

public:
	GreedyOptimizer(OptimizableType model) : model(model), engine(std::mt19937(rd())) {
		indexDistribution = std::uniform_int_distribution<int>(0, model.numberOfParameters-1);
		valueDistribution = std::uniform_int_distribution<int>(1, 5);
		diceRollDistribution = std::uniform_int_distribution<int>(0,2);

	}

	void run() {
		//TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing

		for (int i = 0; i < 10; ++i) {
			do_step();
			model.print_hull(); //TODO: Can't get at it for some reason...
		}
		//TODO: output here?
	}
	
	void do_step() {
	
		double oldFitness = model.fitness();		// store old fitness
		int index = indexDistribution(engine);		// choose parameter to modify
		int oldValue = model.get_parameter(index); 	// store old parameter value	
		int modifier = valueDistribution(engine);		// get random value as a modifier for the parameter
						
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
		if(model.satisfies_constraints() && (model.fitness() > oldFitness || win_dice_roll())) {
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			model.set_parameter(index, oldValue); //revert
		}
	}
private:
	bool win_dice_roll()	{
	
		if (diceRollDistribution(engine) == 0) return true;
		else return false;
	}
};

#endif //GREEDY_OPTIMIZER_H
