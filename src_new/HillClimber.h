#ifndef HILL_CLIMBER_H
#define HILL_CLIMBER_H

#include <cassert>

#include <../../media/data/Boats/hull_optimizer/Optimizer.h>
#include "OptimizableModel.h"

template <typename OptimizableModel >
class HillClimber : public Optimizer
{
public:
	using Model = OptimizableModel;
	Model* model;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine;
	std::uniform_int_distribution<int> indexDistribution;
	std::uniform_int_distribution<int> valueDistribution;
	std::bernoulli_distribution coinFlipDistribution; 
	static constexpr double epsilon = 0.00001;
	
	public:
#ifdef DETERMINISTIC_RUN
	HillClimber(Model* model) : model(model), engine(0), 
#else
	HillClimber(Model* model) : model(model), engine(std::mt19937(rd())),
#endif
		indexDistribution(std::uniform_int_distribution<int>(0, Model::numberOfGenes-1)), 
		valueDistribution(std::uniform_int_distribution<int>(-5, 5)), 
		coinFlipDistribution(std::bernoulli_distribution(0.5)) //adjust probability of accepting worse fitness
		{ }
	
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
		double oldFitness = model->fitness();		// store old fitness
		int index = indexDistribution(engine);		// choose parameter to modify
		int oldValue = model->get_parameter(index); 	// store old parameter value	
		int modifier = valueDistribution(engine);	// get random value as a modifier for the parameter
						
		// modify the parameter
		if (oldValue + modifier > Model::domainHi) { //resulting value too big
			model->set_parameter(index, Model::domainHi);
		} 
		else if (oldValue + modifier < Model::domainLo) { //too small
			model->set_parameter(index, Model::domainLo);
		} 
		else { //can be changed freely
			model->set_parameter(index, oldValue + modifier);
		}
		
		// decide whether or not to keep the new value
		if(model->fitness() > 0.0+epsilon && ( (model->fitness() > oldFitness-epsilon) || coinFlipDistribution(engine))) {
			// keep new solution if valid AND either:
				// (a) is fitter or 
				// (b) you won the dice roll
		} else {
			model->set_parameter(index, oldValue); //revert
		}
// 		model->station_calculator.increment_iteration_no(); //TODO: this line is needed for the MemoizedStationCalculator
	}
};

#endif // HILL_CLIMBER_H
