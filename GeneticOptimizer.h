#ifndef GENETIC_OPTIMIZER_H
#define GENETIC_OPTIMIZER_H

#include <random>
#include <chrono>
#include <cassert>
#include <utility>
#include <cmath>

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
	
#if NDEBUG
	static constexpr unsigned int generations_per_output = 100*1000; //Output every X generations. Note that 0 disables this functionality, outputting only at the beginnign and end.
#else 
	static constexpr unsigned int generations_per_output = 100*1000; //Output every X generations. Note that 0 disables this functionality, outputting only at the beginnign and end.
#endif

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
	
	/* The number of crossovers done per generation */
	static constexpr int number_of_crossovers = population_size/10;
	
	/* This is a measure of selection pressure. The probability for a candidate 
	 * to be selected, irrespective of it's fitness. The fitness-proportional 
	 * component is added ontop of this value.
	 */
	static constexpr double base_selection_chance = 0.3; //default = 0.1
	
	/* This is a measure of selection pressure. Candidates more than x std-devs 
	 * away from the optimal solution only get the base chance of being selected.
	 * Increasing this value means that worse solutions have a high probability of
	 * being chosen, but still proportional to their fitness.
	 */
	static constexpr int selection_threshold_in_std_dev = 5; //default = 2
	
	double population_total_fitness = 0.;
	double population_sum_sqr_fitnesses = 0.;
	double population_best_fitness = 0.;
	double population_worst_fitness = 100.;
	double population_variance = 0.;
	double population_mean_fitness = 0.;
	
	int best_candidate_idx = 0;
	
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
			
			// Selection
			
			//select first parent
			while(true) {
				index = individualDistribution(engine);
				if(selection(index)) {
					first_parent_idx = index;
					break;
				}
			}

			//select second parent
			while(true) {
				index = individualDistribution(engine);
				if(selection(index) && index != first_parent_idx) {
					second_parent_idx = index;
					break;
				}
			}
			
			//select first child
			while(true) {
				index = individualDistribution(engine);
				//Parents, newborns, & best solution of the prev. generation can not be overwritten
				if(population[index].birthday != current_generation && index != first_parent_idx && index != second_parent_idx && index != best_candidate_idx) {
					first_child_idx = index;
					break;
				}
			}
			
			population[first_parent_idx].model.crossover(population[second_parent_idx].model, population[first_child_idx].model);
			population[first_child_idx].model.mutate();
			population[first_child_idx].birthday = current_generation;
			
		}
		
		//iterate over all elements computing their fitness and updating the rolling total
		calculate_population_parameters();
	}
	
	void run(int steps = 10) {
	
		std::chrono::time_point<std::chrono::system_clock> start, end;
		
		output_population_parameters();
		start = std::chrono::system_clock::now();
		
		if(generations_per_output > 0) {
			for (int i = 0; i < steps/generations_per_output; ++i) {	
				for (int j = 0; j<generations_per_output; ++j) {
					do_step();
				}
				output_population_parameters();
			}
		} else {
			for (int i = 0; i < steps; ++i) {
				do_step();
			}
		}
		
		end = std::chrono::system_clock::now();
		
		if (generations_per_output==0) output_population_parameters();
		
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << "Elapsed time = " << elapsed_seconds.count() << std::endl;
	}
	
	inline bool selection(int index) {
		
		static_assert(0. <= base_selection_chance && base_selection_chance <= 1., "The base selection chance must be a probability in [0,1]");
		
		/* Calculates the distance to the best solution in standard deviations. 1 minus this number is the probabilty 
		 * of being selected. Alternatively, the number can be divided by some coeffcieint. This coefficent, the 
		 * selection_threshold_in_std_dev defines just how many standard devs distant a solution must be, for the
		 * std_dev_gradient term to be 0.0
		 */
		double std_dev_gradient = 1 - (population_best_fitness - population[index].model.fitness())*(population_best_fitness - population[index].model.fitness())/((population_variance+0.000001) * selection_threshold_in_std_dev);
		
		/* The std_dev_gradient term is forced to be non-negative, and is scaled so that it plus the base rate make for a 
		 * maximum probabilty of 1, to avoid any saturation of the selection function. */
		std_dev_gradient < 0 ? std_dev_gradient = 0 : (1-base_selection_chance)*std_dev_gradient;
		
		return chanceDistribution(engine) <= (base_selection_chance + std_dev_gradient) && population[index].birthday < current_generation;
		
// 		return chanceDistribution(engine) <= (population[index].model.fitness()/population_best_fitness) && population[index].birthday < current_generation; //old selection function
	}
	
	//Find the best population member, computes the mean & variance
	inline void calculate_population_parameters() {
		best_candidate_idx = 0;
		population_best_fitness = 0;
		population_total_fitness = 0.;
		population_sum_sqr_fitnesses = 0.;
		double K = population[0].model.fitness();
		
		// Using the shifted-data variance algorithm from: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Computing_shifted_data
		for (int i = 0; i<population_size; ++i) {
			double f = population[i].model.fitness();
			population_total_fitness += f - K;
			population_sum_sqr_fitnesses += (f - K) * (f - K);
			if (f > population_best_fitness) {
				population_best_fitness = f;
				best_candidate_idx = i;
			}
		}
		
		population_variance = (population_sum_sqr_fitnesses - (population_total_fitness * population_total_fitness)/population_size)/population_size;
		population_mean_fitness = population_total_fitness/population_size + K;
	}
	
	/* //TODO Calculate the total fitness, mean fitness, and stan0000; //dard deviation and output 
	 * them in a log file together with a time stamp. This method is called at a 
	 * regular but very large interval, so the progress of the optimization can be 
	 * seen.*/
	void output_population_parameters() {
		calculate_population_parameters();
		
		std::cout << "\nGeneration = \t" << current_generation << "\n";
		std::cout << "Mean fitness =\t" << population_mean_fitness << "\n";
		std::cout << "Variance =\t\t" << population_variance << "\n";
		std::cout << "Standard Deviation =\t" << std::sqrt(population_variance) << "\n\n";
		
		std::cout << "Best Candidate Solution" << "\n";
		
		population[best_candidate_idx].model.output();
		population[best_candidate_idx].model.export_hull("GA_" + std::to_string(current_generation));
		//TODO: Output the population parameters like mean, variance, and the file with all the fitness values
	}
	
	//dumps all fitness values into a text file to allow plotting a probability density function for instance
	void output_all_fitnesses(std::string filename) {
		std::ofstream output_file;
		output_file.open("../../data/" + filename + ".txt");
	
		if (output_file.is_open()) {
			
			for (int i=0; i< population_size; ++i) {
				output_file << i << "\t" << population[i].model.fitness() << "\n";
			}
			
			output_file.close();
		} else std::cout << "Error opening file: " << filename << ".dat" << std::endl;
	}
};

#endif //GENETIC_OPTIMIZER_H
