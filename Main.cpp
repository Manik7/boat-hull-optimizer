#include "GreedyOptimizer.h"
#include "MonteCarloOptimizer.h"
#include "OptimizableHull.h"

#include "src_new/HullModel.h"
#include "src_new/HillClimber.h"

void old_alg();
void new_alg();

int main () {
	old_alg();
	
	std::cout << "NEW ALGORITHM\n\n";
	
 	new_alg();
}

void old_alg() {
	OptimizableHull hull;
	hull.compute_properties();
	
	MonteCarloOptimizer<OptimizableHull> carlos(&hull);
	GreedyOptimizer<OptimizableHull> greed(&hull);

#ifdef DETERMINISTIC_RUN
	std::cout << "Deterministic run.\n";
#endif

	std::cout << "Seed\n";
	hull.print_hull();
	hull.export_hull_coordinates("seed.dat");
	
	if (!hull.satisfies_constraints()) {
		carlos.run(1*1000*1000);
		std::cout << "Monte Carlo\n";
		hull.print_hull();
	} else {
		std::cout << "Seed hull OK, skipping Monte Carlo optimization.\n";
	}
	hull.export_hull_coordinates("carlos.dat"); //TODO: Exporting even when monte carlo was not run, because the gnuplot script can't deal with it if carlos.dat is missing
	
	if (hull.satisfies_constraints()) {
		std::cout << "Seed generated successfully! Starting Greed...\n\n";
		greed.run(10*1000*1000);
		std::cout << "Greedy\n";
		hull.print_hull();
		hull.export_hull_coordinates("greed.dat");
	} else {
		std::cout << "Seeding FAILED!\n";
	}
}

void new_alg() {	
	HullModel hull;
	HillClimber<HullModel> hillary(&hull);
	
	hull.output();
	hillary.run(5);
	hull.output();
}
