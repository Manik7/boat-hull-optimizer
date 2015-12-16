#include "GreedyOptimizer.h"
#include "MonteCarloOptimizer.h"
#include "OptimizableHull.h"

int main () {
	OptimizableHull hull;
	hull.generate_stations();
	hull.compute_properties();
	hull.print_hull();
	hull.export_hull_coordinates("seed.dat");
// 	std::cout << "random hull generated, satisfies_constraints() = " << hull.satisfies_constraints() << "\n\n\n";
	
// 	MonteCarloOptimizer<OptimizableHull> carlos(hull);
	GreedyOptimizer<OptimizableHull> greed(hull);
	
// 	carlos.run(1000000);
// 	hull.print_hull();
// 	hull.export_hull_coordinates("carlos.dat");
	
	if (hull.satisfies_constraints()) {
		std::cout << "Seed generated successfully! Starting Greed...\n";		
		
		greed.run(1000000);
		hull.print_hull();
		hull.export_hull_coordinates("greed.dat");
	} else {
		std::cout << "Seeding FAILED!\n";
	}
}
