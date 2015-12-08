#include "GreedyOptimizer.h"

//Constructor
void GreedyOptimizer::GreedyOptimizer : model(model), engine(rd()), indexDistribution(0, model.numberOfParameters-1), valueDistribution(1, 5), diceRollDistribution(0,2) {}

void GreedyOptimizer::run() {

	//TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing

	for (int i = 0; i < 1e6; ++i) {
		do_step();
	}
	
	//TODO: output here?
}

void GreedyOptimizer::do_step() {
	
	double oldFitness = model.fitness();		// store old fitness
	int index = indexDistribution(eng);		// choose parameter to modify
	int oldValue = model.get_parameter(index); 	// store old parameter value	
	int modifier = valueDistribution(eng);		// get random value as a modifier for the parameter
						
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
	if(model.satisfies_constraints() && (model.fitness > oldFitness || win_dice_roll()) {
		// keep new solution if valid AND either:
			// (a) is fitter or 
			// (b) you won the dice roll
	} else {
		model.set_parameter(index, oldValue); //revert
	}
	
}

bool GreedyOptimizer::win_dice_roll() {
	
	if (diceRollDistribution(eng) == 0) return true;
	else return false;
}
