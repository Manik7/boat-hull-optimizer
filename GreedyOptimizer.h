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
	const double epsilon;

public:
#ifdef DETERMINISTIC_RUN
	GreedyOptimizer(OptimizableType model) : model(model), /*engine(std::mt19937(rd())), */ engine(0), 
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-10, 0)), 
		diceRollDistribution(std::uniform_int_distribution<int>(0,100)),
		epsilon(0.00001) { }
#else
	GreedyOptimizer(OptimizableType model) : model(model), engine(std::mt19937(rd())),
		indexDistribution(std::uniform_int_distribution<int>(0, model.numberOfParameters-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-10, 0)), 
		diceRollDistribution(std::uniform_int_distribution<int>(0,100)),
		epsilon(0.00001) { }
#endif

	
		
	void run(int steps = 2) {
		assert(steps > 0);
		//TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing

		for (int i = 0; i < steps; ++i) {
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
			std::cout << "p = " << index << " & resulting value too large\n";					//TODO: remove
			model.set_parameter(index, model.maxValue);
		} 
		else if (oldValue + modifier < model.minValue) { //too small
			std::cout << "p = " << index << " resulting value too small\n";						//TODO: remove
			model.set_parameter(index, model.minValue);
		} 
		else { //can be changed freely
			std::cout << "p = " << index << " can be changed by " << modifier << "\n";				//TODO: remove
			model.set_parameter(index, oldValue + modifier);
		}

		std::cout << "satisfies_constraints() = " << model.satisfies_constraints() << "\n"; 				//TODO: remove
		std::cout << "model.fitness() > oldFitness - epsilon = " << (model.fitness() > oldFitness - epsilon) << "\n"; 	//TODO: remove
		
		// decide whether or not to keep the new value
		if(model.satisfies_constraints() && ( (model.fitness() > oldFitness-epsilon) || win_dice_roll(60))) { //TODO: adjust dice roll probability
//		if(model.satisfies_constraints() && model.fitness() >= oldFitness) {
			if (!model.satisfies_constraints()) std::cout << "won the dice roll\n"; 				//TODO: remove
			std::cout << "keeping solution \n";									//TODO: remove
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			std::cout << "reverting\n"; 										//TODO: remove
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
