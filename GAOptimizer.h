#ifndef GA_OPTIMIZER_H
#define GA_OPTIMIZER_H

#include <random>
#include <chrono>

#include "Optimizer.h"
#include "HullModel.h"

// template <typename OptimizableModel >
class GAOptimizer : public Optimizer {
	
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
	std::bernoulli_distribution coinFlipDistribution;
	
	//Attributes
	ModelType population[population_size];
	unsigned int current_iteration_number = 0;
	
public:
	//TODO: Constructor
	
	
	/*   I. Selection: Stochasitc acceptance to determine set of parents
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
		
		
		
	}
	
	void run(int steps = 5) {
	
		std::chrono::time_point<std::chrono::system_clock> start, end;
		
		//TODO: Find, output and export population-leader
		
		start = std::chrono::system_clock::now();
		for (; current_iteration_number<steps; ++current_iteration_number) {
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

#endif //GA_OPTIMIZER_H