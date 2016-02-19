#include <chrono>

#include "HullModel.h"
#include "HillClimber.h"
#include "GeneticOptimizer.h"

void new_alg(int);
void genetic_alg(int);

int main () {
//  	new_alg(10*1000*1000);
	std::cout << "GENETIC ALGORITHM\n" << std::endl;
#ifdef NDEBUG
	genetic_alg(2*3*1000*1000); //3 million generations take 7 mins (release build)
// 	genetic_alg(24*6*3*1000*1000);
#else
	genetic_alg(100*1000);
#endif
	
}

void new_alg(int runs = 5) {
	HullModel hull;
	HillClimber<HullModel> hillary(&hull);
	
	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	hull.output();
	hull.export_hull("HC_seed.dat");
	start = std::chrono::system_clock::now();
	hillary.run(runs);
	end = std::chrono::system_clock::now();
	hull.output();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout << "Elapsed time = " << elapsed_seconds.count() << std::endl;
	
	hull.export_hull("HC_result.dat");
}

void genetic_alg(int runs = 10) {
	GeneticOptimizer jennifer;
	jennifer.run(runs);
}
