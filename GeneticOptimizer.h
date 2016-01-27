#ifndef GENETIC_OPTIMIZER_H
#define GENETIC_OPTIMIZER_H

#include <random>
#include <chrono>
#include <cassert>
#include <utility>

#include "Optimizer.h"
#include "HullModel.h"

template<typename ModelType>
struct Individual {
	ModelType model;
	int birthday = 0;
	
	Individual() : model(ModelType()) {}
	Individual(ModelType model) : model(model){}
};

// template <typename OptimizableModel >
class GeneticOptimizer : public Optimizer {
	
	/* TODO: OptimizableModel take a good look at the OptimizableModel and 
	 * rename the valueDistribution to "modifierDistrubution" to make the 
	 * name more clear, also make that a normal distribution. 
	 * TODO: I think you should also clean up in that only a single engine 
	 * shoudl be used for everything, i.e. that the crossover function and
	 * the likes use their own (static) distributions for their values, but
	 * the engine should be passed down from the optimizer.
	 */

	using ModelType = HullModel;
	
public:
	static constexpr int population_size = 1000;
// 	static constexpr unsigned int number_of_interim_outputs = 24*6*5;
	static constexpr unsigned int generations_per_output = 200*1000; //Output every X generations. Note that 0 disables this functionality, outputting only at the beginnign and end.

private:
	//Random numbers and distributions
	std::random_device rdevice;
	std::mt19937 engine;
	std::uniform_int_distribution<int> individualDistribution;
	std::uniform_real_distribution<double> chanceDistribution;
	std::bernoulli_distribution coinFlipDistribution;
	
	//Attributes
	Individual<ModelType> population[population_size];
	unsigned int current_generation = 0;
	
	static constexpr int number_of_crossovers = population_size/10;
	
	double population_total_fitness = 0.;
	double population_variance = 0.;
	double population_mean_fitness = 0.;
	
public:
	//TODO: Constructor
#ifdef DETERMINISTIC_RUN
	GeneticOptimizer() : engine(0), 
#else
	GeneticOptimizer() : engine(std::mt19937(rdevice())),
#endif
		individualDistribution(std::uniform_int_distribution<int>(0, population_size-1)),
		chanceDistribution(std::uniform_real_distribution<double>(0.,1.)),
		coinFlipDistribution(std::bernoulli_distribution(0.5)) //adjust probability of accepting worse fitness
	{ 
		ModelType::engine = engine;
		for (int i = 0; i<population_size; ++i) {
			population[i] = Individual<ModelType>(ModelType());
		}
	}
	
	
	/*   I. Selection: Stochastic acceptance to determine set of parents
	 * 
	 *  II. Crossover & Mutation:
	 * 	Iterate over parents (check birthdays to be make sure 
	 * 	they haven't been overwritten by an infant, if they 
	 * 	have, that's too bad, take the next parent from the list),
	 * 	and do a crossover. For the crossover, select 1 (or 2)
	 * 	random members of the population to be overwritten, but 
	 *	ensure that infants are never chosen a victims (check age).
	 * 
	 * III. Fitness evaluation:
	 * 	Iterate over the population to determine each fitness value
	 * 	and the total fitness. 
	 */
	void do_step() {
		++current_generation;
		
		int index;
		int first_parent_idx;
		int second_parent_idx;
		int first_child_idx;
		
		for (int i = 0; i<number_of_crossovers; ++i) {
			
			//select first parent
			while(true) {
				index = individualDistribution(engine);
				if(chanceDistribution(engine) <= 100.*population[index].model.fitness()/population_total_fitness && population[index].birthday != current_generation) {
					first_parent_idx = index;
					break;
				}
			}

			//select second parent
			while(true) {
				index = individualDistribution(engine);
				if(chanceDistribution(engine) <= 100.*population[index].model.fitness()/population_total_fitness && population[index].birthday != current_generation && index != first_parent_idx) {
					second_parent_idx = index;
					break;
				}
			}
			
			//select first child
			while(true) {
				index = individualDistribution(engine);
				if(population[index].birthday != current_generation && index != first_parent_idx && index != second_parent_idx) {
					first_child_idx = index;
					break;
				}
			}
			
			population[first_parent_idx].model.crossover(population[second_parent_idx].model, population[first_child_idx].model);
			population[first_child_idx].model.mutate();
			population[first_child_idx].birthday = current_generation;
			
		}
		
		//iterate over all elements computing their fitness and updating the rolling total
		population_total_fitness = 0.;
		for (auto it : population) {
			population_total_fitness += it.model.fitness();
		}
	}
	
	void run(int steps = 10) {
	
		std::chrono::time_point<std::chrono::system_clock> start, end;
		
		evaluate_population();
		start = std::chrono::system_clock::now();
		
		if(generations_per_output > 0) {
			for (int i = 0; i < steps/generations_per_output; ++i) {	
				for (int j = 0; j<generations_per_output; ++j) {
					do_step();
				}
				evaluate_population();
			}
		} else {
			for (int i = 0; i < steps; ++i) {
				do_step();
			}
		}
		
		end = std::chrono::system_clock::now();
		
		if (generations_per_output==0) evaluate_population();
		
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << "Elapsed time = " << elapsed_seconds.count() << std::endl;
	}
	
	/* //TODO Calculate the total fitness, mean fitness, and standard deviation and output 
	 * them in a log file together with a time stamp. This method is called at a 
	 * regular but very large interval, so the progress of the optimization can be 
	 * seen.*/
	void evaluate_population() {
		
		int best_candidate_idx = 0;
		double best_fitness = 0.0;
		
		population_total_fitness = 0.0;
		
		//TODO: Calculate the standard deviation as well
		for (int i = 0; i<population_size; ++i) {
			population_total_fitness += population[i].model.fitness();
			if (population[i].model.fitness() > best_fitness) {
				best_fitness = population[i].model.fitness();
				best_candidate_idx = i;
			}
		}
		
		population_mean_fitness = population_total_fitness/population_size;
		std::cout << "Mean fitness = " << population_mean_fitness << "\n";
		
		population[best_candidate_idx].model.output();
		population[best_candidate_idx].model.export_hull_coordinates("GA_" + std::to_string(current_generation) + ".dat");
	}
};

#endif //GENETIC_OPTIMIZER_H
