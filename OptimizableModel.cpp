#include "OptimizableModel.h"

std::random_device OptimizableModel::rd;
#ifdef DETERMINISTIC_RUN
std::mt19937 OptimizableModel::engine = std::mt19937(0);
#else
std::mt19937 OptimizableModel::engine = std::mt19937(OptimizableModel::rd());
#endif
std::uniform_int_distribution<int> OptimizableModel::indexDistribution = std::uniform_int_distribution<int>(0, OptimizableModel::numberOfGenes-1);
std::uniform_int_distribution<int> OptimizableModel::valueDistribution = std::uniform_int_distribution<int>(OptimizableModel::domainLo, OptimizableModel::domainHi);
std::normal_distribution<double> OptimizableModel::modifierDistribution = std::normal_distribution<double>(0., OptimizableModel::mutation_standard_deviation);

OptimizableModel::OptimizableModel() {	
	for (int i = 0; i < numberOfGenes; ++i) { 
		genome[i].first = 0;
		genome[i].second = 0;
	}
	
// 	for (int i = 0; i < numberOfGenes; ++i) { 
// 		set_parameter(i, valueDistribution(engine));
// 	}
}

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
