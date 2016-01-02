#ifndef HILL_CLIMBER_H
#define HILL_CLIMBER_H

#include <cassert>

#include <../../media/data/Boats/hull_optimizer/Optimizer.h>
#include "OptimizableModel.h"

template <typename OptimizableModel>
class HillClimber : public Optimizer
{
public:
	OptimizableModel* model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::bernoulli_distribution coinFlipDistribution; 
	static const double epsilon;
	
	public:
#ifdef DETERMINISTIC_RUN
	HillClimber(OptimizableModel* model) : model(model), engine(0), 
#else
	HillClimber(OptimizableModel* model) : model(model), engine(std::mt19937(rd())),
#endif
		indexDistribution(std::uniform_int_distribution<int>(0, OptimizableModel::NUMBER_OF_GENES-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-1, 1)), 
		coinFlipDistribution(std::bernoulli_distribution(0.9)), //adjust dice roll probability
		epsilon(0.00001) { }
	
	void run(int steps = 2) {
		assert(steps > 0);
		
		for (int i = 0; i < steps; ++i) {
			do_step(); //TODO: Implement temperature here, with tuning of the valueDistribution, for simulated annealing
		}
	}
	
	void do_step() {
		/* TODO: OptimizableModel has no mechanism to initialize it sensibly, use the Monte Carlo 
		 * to do it perhaps? -- The problem there though is that Stations etc are partially hardcoded
		 * in the MC methods.
		 */
		double oldFitness = model->fitness;		// store old fitness
		int index = indexDistribution(engine);		// choose parameter to modify
		int oldValue = model->get_parameter(index); 	// store old parameter value	
		int modifier = valueDistribution(engine);	// get random value as a modifier for the parameter
						
		// modify the parameter
		if (oldValue + modifier > OptimizableModel::DOMAIN_HI) { //resulting value too big
			model->set_parameter(index, OptimizableModel::DOMAIN_HI);
		} 
		else if (oldValue + modifier < OptimizableModel::DOMAIN_LO) { //too small
			model->set_parameter(index, OptimizableModel::DOMAIN_LO);
		} 
		else { //can be changed freely
			model->set_parameter(index, oldValue + modifier);
		}
		
		model->compute_fitness();
		
		// decide whether or not to keep the new value
		if(model->fitness > 0.0+epsilon && ( (model->fitness < oldFitness-epsilon) || coinFlipDistribution(engine))) {
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			model->set_parameter(oldValue); //revert
		}
	}
};

#endif // HILL_CLIMBER_H
