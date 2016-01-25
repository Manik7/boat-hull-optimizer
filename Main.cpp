#include <chrono>

#include "HullModel.h"
#include "HillClimber.h"
#include "GeneticOptimizer.h"

void new_alg(int);
void genetic_alg(int);

int main () {
	std::cout << "NEW ALGORITHM\n" << std::endl;
	
 	new_alg(10*1000*1000);
	
// 	genetic_alg(10);
}

void new_alg(int runs = 5) {	
	HullModel hull;
	HillClimber<HullModel> hillary(&hull);
	
	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	hull.output();
	hull.export_hull_coordinates("HC_seed.dat");
	start = std::chrono::system_clock::now();
	hillary.run(runs);
	end = std::chrono::system_clock::now();
	hull.output();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout << "Elapsed time = " << elapsed_seconds.count() << std::endl;
	
	hull.export_hull_coordinates("HC_result.dat");
}

void genetic_alg(int runs = 10) {
	GeneticOptimizer jennifer;
	jennifer.run(runs);
}