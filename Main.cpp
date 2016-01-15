#include "GreedyOptimizer.h"
#include "MonteCarloOptimizer.h"
#include "OptimizableHull.h"

#include "src_new/HullModel.h"
#include "src_new/HillClimber.h"

void old_alg(int);
void new_alg(int);

int main () {
// 	old_alg(10*1000*1000);
	
	std::cout << "NEW ALGORITHM\n" << std::endl;
	
 	new_alg(100*1000*1000);
}

void old_alg(int runs = 5) {
	OptimizableHull hull;
	hull.compute_properties();
	
	MonteCarloOptimizer<OptimizableHull> carlos(&hull);
	GreedyOptimizer<OptimizableHull> greed(&hull);

#ifdef DETERMINISTIC_RUN
	std::cout << "Deterministic run.\n";
#endif

	std::cout << "Seed\n";
	hull.print_hull();
	hull.export_hull_coordinates("G_seed.dat");
	
	if (!hull.satisfies_constraints()) {
		carlos.run(runs);
		std::cout << "Monte Carlo\n";
		hull.print_hull();
	} else {
		std::cout << "Seed hull OK, skipping Monte Carlo optimization.\n";
	}
	hull.export_hull_coordinates("G_carlos.dat"); //TODO: Exporting even when monte carlo was not run, because the gnuplot script can't deal with it if carlos.dat is missing
	
	if (hull.satisfies_constraints()) {
		std::cout << "Seed generated successfully! Starting Greed...\n\n";
		greed.run(10*1000*1000);
		std::cout << "Greedy\n";
		hull.print_hull();
		hull.export_hull_coordinates("G_greed.dat");
	} else {
		std::cout << "Seeding FAILED!\n";
	}
}

void new_alg(int runs = 5) {	
	HullModel hull;
	HillClimber<HullModel> hillary(&hull);
	
	hull.output();
	hull.export_hull_coordinates("HC_seed.dat");
	hillary.run(runs);
	hull.output();
	hull.export_hull_coordinates("HC_result.dat");
}
