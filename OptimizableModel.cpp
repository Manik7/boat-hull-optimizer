#include "OptimizableModel.h"

std::random_device OptimizableModel::rd;
#ifdef DETERMINISTIC_RUN
std::mt19937 OptimizableModel::engine = std::mt19937(0);
#else
std::mt19937 OptimizableModel::engine = std::mt19937(OptimizableModel::rd());
#endif
std::uniform_int_distribution<int> OptimizableModel::indexDistribution = std::uniform_int_distribution<int>(0, OptimizableModel::numberOfGenes-1);
std::uniform_int_distribution<int> OptimizableModel::valueDistribution = std::uniform_int_distribution<int>(OptimizableModel::domainLo, OptimizableModel::domainHi);
std::normal_distribution<double> OptimizableModel::modifierDistribution = std::normal_distribution<double>(0., 0.3);
// std::bernoulli_distribution OptimizableModel::coinFlipDistribution = std::bernoulli_distribution(OptimizableModel::mutationRate);

OptimizableModel::OptimizableModel() /*:
	
#ifdef DETERMINISTIC_RUN
	OptimizableModel(std::mt19937(0))
#else
	OptimizableModel(std::mt19937(OptimizableModel::rd()))
#endif*/
{	
	for (int i = 0; i < numberOfGenes; ++i) { 
		genome[i].first = 0;
		genome[i].second = 0;
	}
	
// 	for (int i = 0; i < numberOfGenes; ++i) { 
// 		set_parameter(i, valueDistribution(engine));
// 	}
}

// OptimizableModel::OptimizableModel(OptimizableModel& model) : OptimizableModel(model.genome, model.engine) {}

// OptimizableModel::OptimizableModel(std::pair<int, NumType> genome[], std::mt19937 engine) : OptimizableModel(engine) {
// 	/* TODO: The values are (and need to be) hard copied. 
// 	 * With an std::vector this would be less of a problem, 
// 	 * and the size could be verified via assertion as well. 
// 	 */
// 	
// 	for (int i = 0; i < numberOfGenes; ++i) { 
// 		this->genome[i] = genome[i];		
// 	}
// 	
// 	assert(isFitnessUpdated == false);
// }

OptimizableModel::OptimizableModel(std::mt19937 engine) : OptimizableModel()/*, engine(engine)*/ {
	engine = engine;
}


double OptimizableModel::fitness() {
	if(isFitnessUpdated) {
		return m_fitness;
	} else {
		isFitnessUpdated = true;
		return m_fitness = compute_fitness();
	}
}

int OptimizableModel::crossover(OptimizableModel& partner, OptimizableModel& child)
{
	int crossover_point = indexDistribution(engine);
	
	int i = 0;
	for ( ; i <= crossover_point && i < numberOfGenes; ++i) { //this one's genes are copied
		child.genome[i] = genome[i];
	}
	for ( ; i < numberOfGenes; ++i) { //partner's gene are copied
		child.genome[i] = partner.genome[i];
	}

// 	//NOTE: The following code packs this into a single loop with an if statement instead of split loops
// 	for (int i = 0; i < numberOfGenes; ++i) { //this one's genes are copied is selected
// 		if (i < crossover_point) {
// 			newGenome[i] = parameters[i];
// 		} else {
// 			newGenome[i] = partner.parameters[i];
// 		}
// 	}
	child.isFitnessUpdated = false;
	return crossover_point;
}

void OptimizableModel::mutate()
{
	for (int index = 0; index < numberOfGenes; ++index) {
// 		if(coinFlipDistribution(engine)) {
			int oldValue = get_parameter(index); 	// store old parameter value	
			int modifier = modifierDistribution(engine);	// get random value as a modifier for the parameter
							
			// modify the parameter
			if (oldValue + modifier > domainHi) { //resulting value too big
				set_parameter(index, domainHi);
			} 
			else if (oldValue + modifier < domainLo) { //too small
				set_parameter(index, domainLo);
			} 
			else { //can be changed freely
				set_parameter(index, oldValue + modifier);
			}
		}
// 	}
}
