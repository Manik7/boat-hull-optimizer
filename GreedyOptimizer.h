#ifndef GREEDY_OPTIMIZER_H
#define GREEDY_OPTIMIZER_H

#include <random>
# include <cassert>
#include <iostream> //TODO: remove
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
#ifdef DETERMINISTIC_RUN
	GreedyOptimizer(OptimizableType model) : model(model), /*engine(std::mt19937(rd())), */ engine(0), 
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-10, 10)), 
		diceRollDistribution(std::uniform_int_distribution<int>(0,100)) { }
#else
	GreedyOptimizer(OptimizableType model) : model(model), engine(std::mt19937(rd())),
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-10, 10)), 
		diceRollDistribution(std::uniform_int_distribution<int>(0,100)) { }
#endif

	
		
	void run() {
		//TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing

		for (int i = 0; i < 10; ++i) {
			do_step();
			model.print_hull();
		}
	}
	
	void do_step() {
	
		double oldFitness = model.fitness();		// store old fitness
		int index = indexDistribution(engine);		// choose parameter to modify
		int oldValue = model.get_parameter(index); 	// store old parameter value	
		int modifier = valueDistribution(engine);		// get random value as a modifier for the parameter
						
		// modify the parameter
		if (oldValue + modifier > model.maxValue) { //resulting value too big
			std::cout << "p = " << index << " & resulting value too large\n";
			model.set_parameter(index, model.maxValue);
		} 
		else if (oldValue + modifier < model.minValue) { //too small
			std::cout << "p = " << index << " resulting value too small\n";
			model.set_parameter(index, model.minValue);
		} 
		else { //can be changed freely
			std::cout << "p = " << index << " can be changed freely\n";
			model.set_parameter(index, oldValue + modifier);
		}

		// decide whether or not to keep the new value
		if(model.satisfies_constraints() && (model.fitness() > oldFitness || win_dice_roll(60))) { //TODO
//		if(model.satisfies_constraints() && model.fitness() >= oldFitness) {
			if (model.satisfies_constraints()) std::cout << "constraints met\n";
			else std::cout << "won the dice roll\n";
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			std::cout << "reverting\n";
			model.set_parameter(index, oldValue); //revert
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
