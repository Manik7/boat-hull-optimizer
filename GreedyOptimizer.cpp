#include "GreedyOptimizer.h"

//Constructor
void GreedyOptimizer::GreedyOptimizer : model(model), engine(rd()), indexDistribution(0, model.numberOfParameters-1), valueDistribution(0, 99), diceRollDistribution(0,2) {}

void GreedyOptimizer::run() {
	//tune the range of the valueDistribution in here, since it is constant, or determined by simulated annealing. You don't have to set it in every step though.
	
	//TODO: Possibly initialize the distribution here for hill climbing (or leave it in the constructor?)
	//TODO: Implement temperature here, for simulated annealing
	//TODO: Tune the valueDistribution here so that it delivers the right values

}

void GreedyOptimizer::do_step() {
	
	double oldFitness = model.fitness();		// store old fitness
	int index = indexDistribution(eng);		// choose parameter to modify
	int oldValue = model.get_parameter(index); 	// store old parameter value	
	int modifier = valueDistribution(eng);		// get random value as a modifier for the parameter
						
	// modify the parameter
	if (oldValue + modifier > maxValue) { //resulting value too big
		model.set_parameter(index, maxValue);
	} 
	else if (oldValue + modifier < minValue) { //too small
		model.set_parameter(index, minValue);
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
