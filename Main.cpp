#include "GreedyOptimizer.h"
#include "MonteCarloOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	MonteCarloOptimizer<OptimizableHull> carlos(hull);
	GreedyOptimizer<OptimizableHull> greed(hull);

	hull.compute_properties();
	hull.print_hull();
	hull.export_hull_coordinates("seed.dat");

	carlos.run(1000000);
	hull.print_hull();
	hull.export_hull_coordinates("carlos.dat");
	
	if (hull.satisfies_constraints()) {
		std::cout << "Seed generated successfully! Starting Greed...\n";
		greed.run(1000000);
		hull.print_hull();
		hull.export_hull_coordinates("greed.dat");
	} else {
		std::cout << "Seeding FAILED!\n";
	}
}
