#ifndef GENETIC_OPTIMIZER_H
#define GENETIC_OPTIMIZER_H

#include <random>
#include <chrono>

#include "Optimizer.h"
#include "HullModel.h"

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

private:
	//Random numbers and distributions
	std::random_device rdevice;
	std::mt19937 engine;
	std::uniform_int_distribution<int> individualDistribution;
	std::uniform_real_distribution<double> chanceDistribution;
	std::bernoulli_distribution coinFlipDistribution;
	
	//Attributes
	std::pair<ModelType, int> population[population_size];
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
		for (int i = 0; i<population_size; ++i) {
			population[i] = std::make_pair(ModelType(engine), 0);
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
			
			//find first parent
			while(true) {
				index = individualDistribution(engine);
				if(chanceDistribution(engine) <= 100.*population[index].first.fitness()/population_total_fitness && population[index].second != current_generation) {
					first_parent_idx = index;
					break;
				}
			}

			//second parent
			while(true) {
				index = individualDistribution(engine);
				if(chanceDistribution(engine) <= 100.*population[index].first.fitness()/population_total_fitness && population[index].second != current_generation && index != first_parent_idx) {
					second_parent_idx = index;
					break;
				}
			}
			
			//first child
			while(true) {
				index = individualDistribution(engine);
				if(population[index].second != current_generation && index != first_parent_idx && index != second_parent_idx) {
					first_child_idx = index;
					break;
				}
			}
			
			population[first_parent_idx].first.crossover(population[second_parent_idx].first, population[first_child_idx].first);
			population[first_child_idx].first.mutate();
			population[first_child_idx].second = current_generation;
			
		}
		
		//TODO: iterate over all elements and compute their fitness (as well as counting up the total fitness)
		
	}
	
	void run(int steps = 5) {
	
		std::chrono::time_point<std::chrono::system_clock> start, end;
		
		//TODO: Find, output and export population-leader
		
		start = std::chrono::system_clock::now();
		for (; current_generation<steps; ++current_generation) {
			//TODO: add output of the population parameters at regular intervals
			do_step();
		}
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << "Elapsed time = " << elapsed_seconds.count() << std::endl;
		
		//TODO: Find, output and export population-leader
	}
	
	/* Calculate the total fitness, mean fitness, and standard deviation and output 
	 * them in a log file together with a time stamp. This method is called at a 
	 * regular but very large interval, so the progress of the optimization can be 
	 * seen.*/
	void calculate_population_parameters(); //TODO
};

#endif //GENETIC_OPTIMIZER_H