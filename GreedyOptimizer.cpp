#include "GreedyOptimizer.h"

//Constructor
void GreedyOptimizer::GreedyOptimizer : model(model) {

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine(rd()); // seed the generator

	std::uniform_int_distribution<int> indexDistribution(0, model.numberOfParameters-1); //set the range over which the parameters should be randomly distributed

	std::uniform_int_distribution<int> valueDistribution(0, 450);
}

void GreedyOptimizer::do_step() {
	
	//TODO: compelte this method

	// - choose parameter to modify
	indexDistribution(eng) ...
	
	// - figure out the allowed range somehow
	// - store old parameter value
	// - figure out by how much you want to modify the parameter and in which direction you want to do that
	// check constraints IF OK then continue, else revert
	// check fitness, IF better then continue, else revert

	//TODO: perhaps it's better to do the entire optimization run as one function call? The whole thign with reverting and all works smoother that way
	
}
